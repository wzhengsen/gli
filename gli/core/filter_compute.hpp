///////////////////////////////////////////////////////////////////////////////////
/// OpenGL Image (gli.g-truc.net)
///
/// Copyright (c) 2008 - 2015 G-Truc Creation (www.g-truc.net)
/// Permission is hereby granted, free of charge, to any person obtaining a copy
/// of this software and associated documentation files (the "Software"), to deal
/// in the Software without restriction, including without limitation the rights
/// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
/// copies of the Software, and to permit persons to whom the Software is
/// furnished to do so, subject to the following conditions:
/// 
/// The above copyright notice and this permission notice shall be included in
/// all copies or substantial portions of the Software.
/// 
/// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
/// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
/// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
/// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
/// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
/// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
/// THE SOFTWARE.
///
/// @ref core
/// @file gli/core/filter.hpp
/// @date 2015-10-18 / 2015-10-18
/// @author Christophe Riccio
///////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "../filter.hpp"
#include "coord.hpp"

namespace gli{
namespace detail
{
	template <typename texture_type, typename samplecoord_type, typename fetch_type, typename texel_type>
	struct filter_base
	{
		typedef texel_type(*filterFunc)(texture_type const & Texture, fetch_type Fetch, samplecoord_type const & SampleCoord, typename texture_type::size_type Layer, typename texture_type::size_type Face, typename texture_type::size_type Level, texel_type const & BorderColor);

		static texel_type nearest
		(
			texture_type const & Texture,
			fetch_type Fetch,
			samplecoord_type const & SampleCoord,
			typename texture_type::size_type Layer,
			typename texture_type::size_type Face,
			typename texture_type::size_type Level,
			texel_type const & BorderColor
		)
		{
			coord_nearest<typename texture_type::texelcoord_type, samplecoord_type> const & Coord = make_coord_nearest(Texture.dimensions(Level), SampleCoord);

			texel_type Texel(BorderColor);
			if(all(Coord.UseTexel))
				Texel = Fetch(Texture, Coord.Texel, Layer, Face, Level);

			return Texel;
		}
	};

	template <typename texture_type, typename samplecoord_type, typename fetch_type, typename texel_type, bool is_float = true>
	struct filter1D : public filter_base<texture_type, samplecoord_type, fetch_type, texel_type>
	{
		static texel_type linear
		(
			texture_type const & Texture,
			fetch_type Fetch,
			samplecoord_type const & SampleCoord,
			typename texture_type::size_type Layer,
			typename texture_type::size_type Face,
			typename texture_type::size_type Level,
			texel_type const & BorderColor
		)
		{
			coord_linear<typename texture_type::texelcoord_type, samplecoord_type> const & Coord = make_coord_linear(Texture.dimensions(Level), SampleCoord);

			texel_type Texel0(BorderColor);
			if(Coord.UseTexelFloor.s)
				Texel0 = Fetch(Texture, typename texture_type::texelcoord_type(Coord.TexelFloor.s), Layer, Face, Level);

			texel_type Texel1(BorderColor);
			if(Coord.UseTexelCeil.s)
				Texel1 = Fetch(Texture, typename texture_type::texelcoord_type(Coord.TexelCeil.s), Layer, Face, Level);

			return mix(Texel0, Texel1, Coord.Blend.s);
		}
	};

	template <typename texture_type, typename samplecoord_type, typename fetch_type, typename texel_type>
	struct filter1D<texture_type, samplecoord_type, fetch_type, texel_type, false> : public filter_base<texture_type, samplecoord_type, fetch_type, texel_type>
	{
		static texel_type linear
		(
			texture_type const & Texture,
			fetch_type Fetch,
			samplecoord_type const & SampleCoord,
			typename texture_type::size_type Layer,
			typename texture_type::size_type Face,
			typename texture_type::size_type Level,
			texel_type const & BorderColor
		)
		{
			return detail::filter1D<texture_type, samplecoord_type, fetch_type, texel_type, false>::nearest(Texture, Fetch, SampleCoord, Layer, Face, Level, BorderColor);
		}
	};

	template <typename texture_type, typename samplecoord_type, typename fetch_type, typename texel_type, bool is_float = true>
	struct filter2D : public filter_base<texture_type, samplecoord_type, fetch_type, texel_type>
	{
		static texel_type linear
		(
			texture_type const & Texture,
			fetch_type Fetch,
			samplecoord_type const & SampleCoordWrap,
			typename texture_type::size_type Layer,
			typename texture_type::size_type Face,
			typename texture_type::size_type Level,
			texel_type const & BorderColor
		)
		{
			coord_linear<typename texture_type::texelcoord_type, samplecoord_type> const & Coord = make_coord_linear(Texture.dimensions(Level), SampleCoordWrap);

			texel_type Texel00(BorderColor);
			if(Coord.UseTexelFloor.s && Coord.UseTexelFloor.t)
				Texel00 = Fetch(Texture, typename texture_type::texelcoord_type(Coord.TexelFloor.s, Coord.TexelFloor.t), Layer, Face, Level);

			texel_type Texel10(BorderColor);
			if(Coord.UseTexelCeil.s && Coord.UseTexelFloor.t)
				Texel10 = Fetch(Texture, typename texture_type::texelcoord_type(Coord.TexelCeil.s, Coord.TexelFloor.t), Layer, Face, Level);

			texel_type Texel11(BorderColor);
			if(Coord.UseTexelCeil.s && Coord.UseTexelCeil.t)
				Texel11 = Fetch(Texture, typename texture_type::texelcoord_type(Coord.TexelCeil.s, Coord.TexelCeil.t), Layer, Face, Level);

			texel_type Texel01(BorderColor);
			if(Coord.UseTexelFloor.s && Coord.UseTexelCeil.t)
				Texel01 = Fetch(Texture, typename texture_type::texelcoord_type(Coord.TexelFloor.s, Coord.TexelCeil.t), Layer, Face, Level);

			texel_type const ValueA(mix(Texel00, Texel10, Coord.Blend.s));
			texel_type const ValueB(mix(Texel01, Texel11, Coord.Blend.s));
			return mix(ValueA, ValueB, Coord.Blend.t);
		}

		static texel_type linear_mipmap_linear
		(
			texture_type const & Texture,
			fetch_type Fetch,
			samplecoord_type const & SampleCoordWrap,
			typename texture_type::size_type Layer,
			typename texture_type::size_type Face,
			typename texture_type::size_type Level,
			texel_type const & BorderColor
		)
		{
			texel_type const MinTexel = linear(Texture, Convert.Fetch, SampleCoordWrap, size_type(0), Face, size_type(floor(Level)), BorderColor);
			texel_type const MaxTexel = linear(Texture, Convert.Fetch, SampleCoordWrap, size_type(0), Face, size_type(ceil(Level)), BorderColor);
			return mix(MinTexel, MaxTexel, fract(Level));
		}

		static texel_type linear_mipmap_nearest
		(
			texture_type const & Texture,
			fetch_type Fetch,
			samplecoord_type const & SampleCoordWrap,
			typename texture_type::size_type Layer,
			typename texture_type::size_type Face,
			typename texture_type::size_type Level,
			texel_type const & BorderColor
		)
		{
			return linear(Texture, Convert.Fetch, SampleCoordWrap, size_type(0), Face, size_type(round(Level)), BorderColor);
		}

		static texel_type nearest_mipmap_linear
		(
			texture_type const & Texture,
			fetch_type Fetch,
			samplecoord_type const & SampleCoordWrap,
			typename texture_type::size_type Layer,
			typename texture_type::size_type Face,
			typename texture_type::size_type Level,
			texel_type const & BorderColor
		)
		{
			texel_type const MinTexel = nearest(Texture, Convert.Fetch, SampleCoordWrap, size_type(0), Face, size_type(floor(Level)), BorderColor);
			texel_type const MaxTexel = nearest(Texture, Convert.Fetch, SampleCoordWrap, size_type(0), Face, size_type(ceil(Level)), BorderColor);
			return mix(MinTexel, MaxTexel, fract(Level));
		}
	};

	template <typename texture_type, typename samplecoord_type, typename fetch_type, typename texel_type>
	struct filter2D<texture_type, samplecoord_type, fetch_type, texel_type, false> : public filter_base<texture_type, samplecoord_type, fetch_type, texel_type>
	{
		static texel_type linear
		(
			texture_type const & Texture,
			fetch_type Fetch,
			samplecoord_type const & SampleCoord,
			typename texture_type::size_type Layer,
			typename texture_type::size_type Face,
			typename texture_type::size_type Level,
			texel_type const & BorderColor
		)
		{
			return detail::filter2D<texture_type, samplecoord_type, fetch_type, texel_type, false>::nearest(Texture, Fetch, SampleCoord, Layer, Face, Level, BorderColor);
		}
	};

	template <typename texture_type, typename samplecoord_type, typename fetch_type, typename texel_type, bool is_float = true>
	struct filter3D : public filter_base<texture_type, samplecoord_type, fetch_type, texel_type>
	{
		static texel_type linear
		(
			texture_type const & Texture,
			fetch_type Fetch,
			samplecoord_type const & SampleCoord,
			typename texture_type::size_type Layer,
			typename texture_type::size_type Face,
			typename texture_type::size_type Level,
			texel_type const & BorderColor
		)
		{
			coord_linear<typename texture_type::texelcoord_type, samplecoord_type> const & Coord = make_coord_linear(Texture.dimensions(Level), SampleCoord);

			texel_type Texel000(BorderColor);
			if(Coord.UseTexelFloor.s && Coord.UseTexelFloor.t && Coord.UseTexelFloor.p)
				Texel000 = Fetch(Texture, typename texture_type::texelcoord_type(Coord.TexelFloor.s, Coord.TexelFloor.t, Coord.TexelFloor.p), Layer, Face, Level);

			texel_type Texel100(BorderColor);
			if(Coord.UseTexelCeil.s && Coord.UseTexelFloor.t && Coord.UseTexelFloor.p)
				Texel100 = Fetch(Texture, typename texture_type::texelcoord_type(Coord.TexelCeil.s, Coord.TexelFloor.t, Coord.TexelFloor.p), Layer, Face, Level);

			texel_type Texel110(BorderColor);
			if(Coord.UseTexelCeil.s && Coord.UseTexelCeil.t && Coord.UseTexelFloor.p)
				Texel110 = Fetch(Texture, typename texture_type::texelcoord_type(Coord.TexelCeil.s, Coord.TexelCeil.t, Coord.TexelFloor.p), Layer, Face, Level);

			texel_type Texel010(BorderColor);
			if(Coord.UseTexelFloor.s && Coord.UseTexelCeil.t && Coord.UseTexelFloor.p)
				Texel010 = Fetch(Texture, typename texture_type::texelcoord_type(Coord.TexelFloor.s, Coord.TexelCeil.t, Coord.TexelFloor.p), Layer, Face, Level);

			texel_type Texel001(BorderColor);
			if (Coord.UseTexelFloor.s && Coord.UseTexelFloor.t && Coord.UseTexelCeil.p)
				Texel001 = Fetch(Texture, typename texture_type::texelcoord_type(Coord.TexelFloor.s, Coord.TexelFloor.t, Coord.TexelCeil.p), Layer, Face, Level);

			texel_type Texel101(BorderColor);
			if(Coord.UseTexelCeil.s && Coord.UseTexelFloor.t && Coord.UseTexelCeil.p)
				Texel101 = Fetch(Texture, typename texture_type::texelcoord_type(Coord.TexelCeil.s, Coord.TexelFloor.t, Coord.TexelCeil.p), Layer, Face, Level);

			texel_type Texel111(BorderColor);
			if(Coord.UseTexelCeil.s && Coord.UseTexelCeil.t && Coord.UseTexelCeil.p)
				Texel111 = Fetch(Texture, typename texture_type::texelcoord_type(Coord.TexelCeil.s, Coord.TexelCeil.t, Coord.TexelCeil.p), Layer, Face, Level);

			texel_type Texel011(BorderColor);
			if(Coord.UseTexelFloor.s && Coord.UseTexelCeil.t && Coord.UseTexelCeil.p)
				Texel011 = Fetch(Texture, typename texture_type::texelcoord_type(Coord.TexelFloor.s, Coord.TexelCeil.t, Coord.TexelCeil.p), Layer, Face, Level);

			texel_type const ValueA(mix(Texel000, Texel100, Coord.Blend.s));
			texel_type const ValueB(mix(Texel010, Texel110, Coord.Blend.s));

			texel_type const ValueC(mix(Texel001, Texel101, Coord.Blend.s));
			texel_type const ValueD(mix(Texel011, Texel111, Coord.Blend.s));

			texel_type const ValueE(mix(ValueA, ValueB, Coord.Blend.t));
			texel_type const ValueF(mix(ValueC, ValueD, Coord.Blend.t));

			return mix(ValueE, ValueF, Coord.Blend.p);
		}
	};

	template <typename texture_type, typename samplecoord_type, typename fetch_type, typename texel_type>
	struct filter3D<texture_type, samplecoord_type, fetch_type, texel_type, false> : public filter_base<texture_type, samplecoord_type, fetch_type, texel_type>
	{
		static texel_type linear
		(
			texture_type const & Texture,
			fetch_type Fetch,
			samplecoord_type const & SampleCoord,
			typename texture_type::size_type Layer,
			typename texture_type::size_type Face,
			typename texture_type::size_type Level,
			texel_type const & BorderColor
		)
		{
			return detail::filter3D<texture_type, samplecoord_type, fetch_type, texel_type, false>::nearest(Texture, Fetch, SampleCoord, Layer, Face, Level, BorderColor);
		}
	};
}//namespace detail
}//namespace gli

