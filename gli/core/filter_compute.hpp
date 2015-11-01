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
	template <typename filter_type>
	typename filter_type::filterFunc get_filter_func(filter Mip, filter Min)
	{
		static typename filter_type::filterFunc Table[][FILTER_COUNT] =
		{
			{
				nullptr,
				nullptr,
				nullptr
			},
			{
				nullptr,
				filter_type::nearest_mipmap_nearest,
				filter_type::linear_mipmap_nearest,
			},
			{
				nullptr,
				filter_type::nearest_mipmap_linear,
				filter_type::linear_mipmap_linear
			}
		};
		static_assert(sizeof(Table) / sizeof(Table[0]) == FILTER_COUNT, "GLI ERROR: 'Table' doesn't match the number of supported filters");

		GLI_ASSERT(Table[Mip][Min]);

		return Table[Mip][Min];
	}

	template <typename T>
	struct interpolate
	{
		typedef float type;
	};

	template <>
	struct interpolate<double>
	{
		typedef double type;
	};

	template <>
	struct interpolate<long double>
	{
		typedef long double type;
	};

	template <typename texture_type, typename interpolate_type, typename samplecoord_type, typename fetch_type, typename texel_type>
	struct filter_base
	{
		typedef texel_type(*filterFunc)(
			texture_type const & Texture, fetch_type Fetch, samplecoord_type const & SampleCoordWrap,
			typename texture_type::size_type Layer, typename texture_type::size_type Face, interpolate_type Level,
			texel_type const & BorderColor);

		static texel_type nearest
		(
			texture_type const & Texture, fetch_type Fetch, samplecoord_type const & SampleCoordWrap,
			typename texture_type::size_type Layer, typename texture_type::size_type Face, interpolate_type Level,
			texel_type const & BorderColor
		)
		{
			typename texture_type::size_type const LevelIndex = static_cast<typename texture_type::size_type>(Level);
			coord_nearest<typename texture_type::texelcoord_type, samplecoord_type> const & Coord = make_coord_nearest(Texture.dimensions(LevelIndex), SampleCoordWrap);

			texel_type Texel(BorderColor);
			if(all(Coord.UseTexel))
				Texel = Fetch(Texture, Coord.Texel, Layer, Face, LevelIndex);

			return Texel;
		}
	};

	template <typename texture_type, typename interpolate_type, typename samplecoord_type, typename fetch_type, typename texel_type, bool is_float = true>
	struct filter1D : public filter_base<texture_type, interpolate_type, samplecoord_type, fetch_type, texel_type>
	{
		typedef filter_base<texture_type, interpolate_type, samplecoord_type, fetch_type, texel_type> base_type;

		static texel_type linear
		(
			texture_type const & Texture, fetch_type Fetch, samplecoord_type const & SampleCoordWrap,
			typename texture_type::size_type Layer, typename texture_type::size_type Face, interpolate_type Level,
			texel_type const & BorderColor
		)
		{
			typename texture_type::size_type const LevelIndex = static_cast<typename texture_type::size_type>(Level);
			coord_linear<typename texture_type::texelcoord_type, samplecoord_type> const & Coord = make_coord_linear(Texture.dimensions(LevelIndex), SampleCoordWrap);

			texel_type Texel0(BorderColor);
			if(Coord.UseTexelFloor.s)
				Texel0 = Fetch(Texture, typename texture_type::texelcoord_type(Coord.TexelFloor.s), Layer, Face, LevelIndex);

			texel_type Texel1(BorderColor);
			if(Coord.UseTexelCeil.s)
				Texel1 = Fetch(Texture, typename texture_type::texelcoord_type(Coord.TexelCeil.s), Layer, Face, LevelIndex);

			return mix(Texel0, Texel1, Coord.Blend.s);
		}

		static texel_type linear_mipmap_linear
		(
			texture_type const & Texture, fetch_type Fetch, samplecoord_type const & SampleCoordWrap,
			typename texture_type::size_type Layer, typename texture_type::size_type Face, interpolate_type Level,
			texel_type const & BorderColor
		)
		{
			texel_type const MinTexel = linear(Texture, Fetch, SampleCoordWrap, Layer, Face, floor(Level), BorderColor);
			texel_type const MaxTexel = linear(Texture, Fetch, SampleCoordWrap, Layer, Face, ceil(Level), BorderColor);
			return mix(MinTexel, MaxTexel, fract(Level));
		}

		static texel_type linear_mipmap_nearest
		(
			texture_type const & Texture, fetch_type Fetch, samplecoord_type const & SampleCoordWrap,
			typename texture_type::size_type Layer, typename texture_type::size_type Face, interpolate_type Level,
			texel_type const & BorderColor
		)
		{
			return linear(Texture, Fetch, SampleCoordWrap, Layer, Face, round(Level), BorderColor);
		}

		static texel_type nearest_mipmap_linear
		(
			texture_type const & Texture, fetch_type Fetch, samplecoord_type const & SampleCoordWrap,
			typename texture_type::size_type Layer, typename texture_type::size_type Face, interpolate_type Level,
			texel_type const & BorderColor
		)
		{
			texel_type const MinTexel = base_type::nearest(Texture, Fetch, SampleCoordWrap, Layer, Face, floor(Level), BorderColor);
			texel_type const MaxTexel = base_type::nearest(Texture, Fetch, SampleCoordWrap, Layer, Face, ceil(Level), BorderColor);
			return mix(MinTexel, MaxTexel, fract(Level));
		}

		static texel_type nearest_mipmap_nearest
		(
			texture_type const & Texture, fetch_type Fetch, samplecoord_type const & SampleCoordWrap,
			typename texture_type::size_type Layer, typename texture_type::size_type Face, interpolate_type Level,
			texel_type const & BorderColor
		)
		{
			return base_type::nearest(Texture, Fetch, SampleCoordWrap, Layer, Face, round(Level), BorderColor);
		}
	};

	template <typename texture_type, typename interpolate_type, typename samplecoord_type, typename fetch_type, typename texel_type>
	struct filter1D<texture_type, interpolate_type, samplecoord_type, fetch_type, texel_type, false> : public filter_base<texture_type, interpolate_type, samplecoord_type, fetch_type, texel_type>
	{
		typedef filter_base<texture_type, interpolate_type, samplecoord_type, fetch_type, texel_type> base_type;

		static texel_type linear
		(
			texture_type const & Texture, interpolate_type Fetch, samplecoord_type const & SampleCoord,
			typename texture_type::size_type Layer, typename texture_type::size_type Face, interpolate_type Level,
			texel_type const & BorderColor
		)
		{
			return base_type::nearest(Texture, Fetch, SampleCoord, Layer, Face, Level, BorderColor);
		}

		static texel_type linear_mipmap_linear
		(
			texture_type const & Texture, fetch_type Fetch, samplecoord_type const & SampleCoordWrap,
			typename texture_type::size_type Layer, typename texture_type::size_type Face, interpolate_type Level,
			texel_type const & BorderColor
		)
		{
			return base_type::nearest(Texture, Fetch, SampleCoordWrap, Layer, Face, round(Level), BorderColor);
		}

		static texel_type linear_mipmap_nearest
		(
			texture_type const & Texture, fetch_type Fetch, samplecoord_type const & SampleCoordWrap,
			typename texture_type::size_type Layer, typename texture_type::size_type Face, interpolate_type Level,
			texel_type const & BorderColor
		)
		{
			return base_type::nearest(Texture, Fetch, SampleCoordWrap, Layer, Face, round(Level), BorderColor);
		}

		static texel_type nearest_mipmap_linear
		(
			texture_type const & Texture, fetch_type Fetch, samplecoord_type const & SampleCoordWrap,
			typename texture_type::size_type Layer, typename texture_type::size_type Face, interpolate_type Level,
			texel_type const & BorderColor
		)
		{
			return base_type::nearest(Texture, Fetch, SampleCoordWrap, Layer, Face, round(Level), BorderColor);
		}

		static texel_type nearest_mipmap_nearest
		(
			texture_type const & Texture, fetch_type Fetch, samplecoord_type const & SampleCoordWrap,
			typename texture_type::size_type Layer, typename texture_type::size_type Face, interpolate_type Level,
			texel_type const & BorderColor
		)
		{
			return base_type::nearest(Texture, Fetch, SampleCoordWrap, Layer, Face, round(Level), BorderColor);
		}
	};

	template <typename texture_type, typename interpolate_type, typename samplecoord_type, typename fetch_type, typename texel_type, bool is_float = true>
	struct filter2D : public filter_base<texture_type, interpolate_type, samplecoord_type, fetch_type, texel_type>
	{
		typedef filter_base<texture_type, interpolate_type, samplecoord_type, fetch_type, texel_type> base_type;

		static texel_type linear
		(
			texture_type const & Texture, fetch_type Fetch, samplecoord_type const & SampleCoordWrap,
			typename texture_type::size_type Layer, typename texture_type::size_type Face, interpolate_type Level,
			texel_type const & BorderColor
		)
		{
			typename texture_type::size_type const LevelIndex = static_cast<typename texture_type::size_type>(Level);
			coord_linear<typename texture_type::texelcoord_type, samplecoord_type> const & Coord = make_coord_linear(Texture.dimensions(LevelIndex), SampleCoordWrap);

			texel_type Texel00(BorderColor);
			if(Coord.UseTexelFloor.s && Coord.UseTexelFloor.t)
				Texel00 = Fetch(Texture, typename texture_type::texelcoord_type(Coord.TexelFloor.s, Coord.TexelFloor.t), Layer, Face, LevelIndex);

			texel_type Texel10(BorderColor);
			if(Coord.UseTexelCeil.s && Coord.UseTexelFloor.t)
				Texel10 = Fetch(Texture, typename texture_type::texelcoord_type(Coord.TexelCeil.s, Coord.TexelFloor.t), Layer, Face, LevelIndex);

			texel_type Texel11(BorderColor);
			if(Coord.UseTexelCeil.s && Coord.UseTexelCeil.t)
				Texel11 = Fetch(Texture, typename texture_type::texelcoord_type(Coord.TexelCeil.s, Coord.TexelCeil.t), Layer, Face, LevelIndex);

			texel_type Texel01(BorderColor);
			if(Coord.UseTexelFloor.s && Coord.UseTexelCeil.t)
				Texel01 = Fetch(Texture, typename texture_type::texelcoord_type(Coord.TexelFloor.s, Coord.TexelCeil.t), Layer, Face, LevelIndex);
/*
			texel_type const & Texel00 = Fetch(Texture, typename texture_type::texelcoord_type(Coord.TexelFloor.s, Coord.TexelFloor.t), Layer, Face, LevelIndex);
			texel_type const & Texel10 = Fetch(Texture, typename texture_type::texelcoord_type(Coord.TexelCeil.s, Coord.TexelFloor.t), Layer, Face, LevelIndex);
			texel_type const & Texel11 = Fetch(Texture, typename texture_type::texelcoord_type(Coord.TexelCeil.s, Coord.TexelCeil.t), Layer, Face, LevelIndex);
			texel_type const & Texel01 = Fetch(Texture, typename texture_type::texelcoord_type(Coord.TexelFloor.s, Coord.TexelCeil.t), Layer, Face, LevelIndex);
*/
/*
			texel_type const & Texel00 = Texture.load<vec4>(typename texture_type::texelcoord_type(Coord.TexelFloor.s, Coord.TexelFloor.t), LevelIndex);
			texel_type const & Texel10 = Texture.load<vec4>(typename texture_type::texelcoord_type(Coord.TexelCeil.s, Coord.TexelFloor.t), LevelIndex);
			texel_type const & Texel11 = Texture.load<vec4>(typename texture_type::texelcoord_type(Coord.TexelCeil.s, Coord.TexelCeil.t), LevelIndex);
			texel_type const & Texel01 = Texture.load<vec4>(typename texture_type::texelcoord_type(Coord.TexelFloor.s, Coord.TexelCeil.t), LevelIndex);
*/
			texel_type const ValueA(mix(Texel00, Texel10, Coord.Blend.s));
			texel_type const ValueB(mix(Texel01, Texel11, Coord.Blend.s));
			return mix(ValueA, ValueB, Coord.Blend.t);
		}

		static texel_type linear_mipmap_linear
		(
			texture_type const & Texture, fetch_type Fetch, samplecoord_type const & SampleCoordWrap,
			typename texture_type::size_type Layer, typename texture_type::size_type Face, interpolate_type Level,
			texel_type const & BorderColor
		)
		{
			texel_type const MinTexel = linear(Texture, Fetch, SampleCoordWrap, Layer, Face, floor(Level), BorderColor);
			texel_type const MaxTexel = linear(Texture, Fetch, SampleCoordWrap, Layer, Face, ceil(Level), BorderColor);
			return mix(MinTexel, MaxTexel, fract(Level));
		}

		static texel_type linear_mipmap_nearest
		(
			texture_type const & Texture, fetch_type Fetch, samplecoord_type const & SampleCoordWrap,
			typename texture_type::size_type Layer, typename texture_type::size_type Face, interpolate_type Level,
			texel_type const & BorderColor
		)
		{
			return linear(Texture, Fetch, SampleCoordWrap, Layer, Face, round(Level), BorderColor);
		}

		static texel_type nearest_mipmap_linear
		(
			texture_type const & Texture, fetch_type Fetch, samplecoord_type const & SampleCoordWrap,
			typename texture_type::size_type Layer, typename texture_type::size_type Face, interpolate_type Level,
			texel_type const & BorderColor
		)
		{
			texel_type const MinTexel = base_type::nearest(Texture, Fetch, SampleCoordWrap, Layer, Face, floor(Level), BorderColor);
			texel_type const MaxTexel = base_type::nearest(Texture, Fetch, SampleCoordWrap, Layer, Face, ceil(Level), BorderColor);
			return mix(MinTexel, MaxTexel, fract(Level));
		}

		static texel_type nearest_mipmap_nearest
		(
			texture_type const & Texture, fetch_type Fetch, samplecoord_type const & SampleCoordWrap,
			typename texture_type::size_type Layer, typename texture_type::size_type Face, interpolate_type Level,
			texel_type const & BorderColor
		)
		{
			return base_type::nearest(Texture, Fetch, SampleCoordWrap, Layer, Face, round(Level), BorderColor);
		}
	};

	template <typename texture_type, typename interpolate_type, typename samplecoord_type, typename fetch_type, typename texel_type>
	struct filter2D<texture_type, interpolate_type, samplecoord_type, fetch_type, texel_type, false> : public filter_base<texture_type, interpolate_type, samplecoord_type, fetch_type, texel_type>
	{
		typedef filter_base<texture_type, interpolate_type, samplecoord_type, fetch_type, texel_type> base_type;

		static texel_type linear
		(
			texture_type const & Texture, fetch_type Fetch, samplecoord_type const & SampleCoord,
			typename texture_type::size_type Layer, typename texture_type::size_type Face, interpolate_type Level,
			texel_type const & BorderColor
		)
		{
			return base_type::nearest(Texture, Fetch, SampleCoord, Layer, Face, Level, BorderColor);
		}

		static texel_type linear_mipmap_linear
		(
			texture_type const & Texture, fetch_type Fetch, samplecoord_type const & SampleCoordWrap,
			typename texture_type::size_type Layer, typename texture_type::size_type Face, interpolate_type Level,
			texel_type const & BorderColor
		)
		{
			return base_type::nearest(Texture, Fetch, SampleCoordWrap, Layer, Face, round(Level), BorderColor);
		}

		static texel_type linear_mipmap_nearest
		(
			texture_type const & Texture, fetch_type Fetch, samplecoord_type const & SampleCoordWrap,
			typename texture_type::size_type Layer, typename texture_type::size_type Face, interpolate_type Level,
			texel_type const & BorderColor
		)
		{
			return base_type::nearest(Texture, Fetch, SampleCoordWrap, Layer, Face, round(Level), BorderColor);
		}

		static texel_type nearest_mipmap_linear
		(
			texture_type const & Texture, fetch_type Fetch, samplecoord_type const & SampleCoordWrap,
			typename texture_type::size_type Layer, typename texture_type::size_type Face, interpolate_type Level,
			texel_type const & BorderColor
		)
		{
			return base_type::nearest(Texture, Fetch, SampleCoordWrap, Layer, Face, round(Level), BorderColor);
		}

		static texel_type nearest_mipmap_nearest
		(
			texture_type const & Texture, fetch_type Fetch, samplecoord_type const & SampleCoordWrap,
			typename texture_type::size_type Layer, typename texture_type::size_type Face, interpolate_type Level,
			texel_type const & BorderColor
		)
		{
			return base_type::nearest(Texture, Fetch, SampleCoordWrap, Layer, Face, round(Level), BorderColor);
		}
	};

	template <typename texture_type, typename interpolate_type, typename samplecoord_type, typename fetch_type, typename texel_type, bool is_float = true>
	struct filter3D : public filter_base<texture_type, interpolate_type, samplecoord_type, fetch_type, texel_type>
	{
		typedef filter_base<texture_type, interpolate_type, samplecoord_type, fetch_type, texel_type> base_type;

		static texel_type linear
		(
			texture_type const & Texture, fetch_type Fetch, samplecoord_type const & SampleCoord,
			typename texture_type::size_type Layer, typename texture_type::size_type Face, interpolate_type Level,
			texel_type const & BorderColor
		)
		{
			typename texture_type::size_type const LevelIndex = static_cast<typename texture_type::size_type>(Level);
			coord_linear<typename texture_type::texelcoord_type, samplecoord_type> const & Coord = make_coord_linear(Texture.dimensions(LevelIndex), SampleCoord);

			texel_type Texel000(BorderColor);
			if(Coord.UseTexelFloor.s && Coord.UseTexelFloor.t && Coord.UseTexelFloor.p)
				Texel000 = Fetch(Texture, typename texture_type::texelcoord_type(Coord.TexelFloor.s, Coord.TexelFloor.t, Coord.TexelFloor.p), Layer, Face, LevelIndex);

			texel_type Texel100(BorderColor);
			if(Coord.UseTexelCeil.s && Coord.UseTexelFloor.t && Coord.UseTexelFloor.p)
				Texel100 = Fetch(Texture, typename texture_type::texelcoord_type(Coord.TexelCeil.s, Coord.TexelFloor.t, Coord.TexelFloor.p), Layer, Face, LevelIndex);

			texel_type Texel110(BorderColor);
			if(Coord.UseTexelCeil.s && Coord.UseTexelCeil.t && Coord.UseTexelFloor.p)
				Texel110 = Fetch(Texture, typename texture_type::texelcoord_type(Coord.TexelCeil.s, Coord.TexelCeil.t, Coord.TexelFloor.p), Layer, Face, LevelIndex);

			texel_type Texel010(BorderColor);
			if(Coord.UseTexelFloor.s && Coord.UseTexelCeil.t && Coord.UseTexelFloor.p)
				Texel010 = Fetch(Texture, typename texture_type::texelcoord_type(Coord.TexelFloor.s, Coord.TexelCeil.t, Coord.TexelFloor.p), Layer, Face, LevelIndex);

			texel_type Texel001(BorderColor);
			if (Coord.UseTexelFloor.s && Coord.UseTexelFloor.t && Coord.UseTexelCeil.p)
				Texel001 = Fetch(Texture, typename texture_type::texelcoord_type(Coord.TexelFloor.s, Coord.TexelFloor.t, Coord.TexelCeil.p), Layer, Face, LevelIndex);

			texel_type Texel101(BorderColor);
			if(Coord.UseTexelCeil.s && Coord.UseTexelFloor.t && Coord.UseTexelCeil.p)
				Texel101 = Fetch(Texture, typename texture_type::texelcoord_type(Coord.TexelCeil.s, Coord.TexelFloor.t, Coord.TexelCeil.p), Layer, Face, LevelIndex);

			texel_type Texel111(BorderColor);
			if(Coord.UseTexelCeil.s && Coord.UseTexelCeil.t && Coord.UseTexelCeil.p)
				Texel111 = Fetch(Texture, typename texture_type::texelcoord_type(Coord.TexelCeil.s, Coord.TexelCeil.t, Coord.TexelCeil.p), Layer, Face, LevelIndex);

			texel_type Texel011(BorderColor);
			if(Coord.UseTexelFloor.s && Coord.UseTexelCeil.t && Coord.UseTexelCeil.p)
				Texel011 = Fetch(Texture, typename texture_type::texelcoord_type(Coord.TexelFloor.s, Coord.TexelCeil.t, Coord.TexelCeil.p), Layer, Face, LevelIndex);

			texel_type const ValueA(mix(Texel000, Texel100, Coord.Blend.s));
			texel_type const ValueB(mix(Texel010, Texel110, Coord.Blend.s));

			texel_type const ValueC(mix(Texel001, Texel101, Coord.Blend.s));
			texel_type const ValueD(mix(Texel011, Texel111, Coord.Blend.s));

			texel_type const ValueE(mix(ValueA, ValueB, Coord.Blend.t));
			texel_type const ValueF(mix(ValueC, ValueD, Coord.Blend.t));

			return mix(ValueE, ValueF, Coord.Blend.p);
		}

		static texel_type linear_mipmap_linear
		(
			texture_type const & Texture, fetch_type Fetch, samplecoord_type const & SampleCoordWrap,
			typename texture_type::size_type Layer, typename texture_type::size_type Face, interpolate_type Level,
			texel_type const & BorderColor
		)
		{
			texel_type const MinTexel = linear(Texture, Fetch, SampleCoordWrap, Layer, Face, floor(Level), BorderColor);
			texel_type const MaxTexel = linear(Texture, Fetch, SampleCoordWrap, Layer, Face, ceil(Level), BorderColor);
			return mix(MinTexel, MaxTexel, fract(Level));
		}

		static texel_type linear_mipmap_nearest
		(
			texture_type const & Texture, fetch_type Fetch, samplecoord_type const & SampleCoordWrap,
			typename texture_type::size_type Layer, typename texture_type::size_type Face, interpolate_type Level,
			texel_type const & BorderColor
		)
		{
			return linear(Texture, Fetch, SampleCoordWrap, Layer, Face, round(Level), BorderColor);
		}

		static texel_type nearest_mipmap_linear
		(
			texture_type const & Texture, fetch_type Fetch, samplecoord_type const & SampleCoordWrap,
			typename texture_type::size_type Layer, typename texture_type::size_type Face, interpolate_type Level,
			texel_type const & BorderColor
		)
		{
			texel_type const MinTexel = base_type::nearest(Texture, Fetch, SampleCoordWrap, Layer, Face, floor(Level), BorderColor);
			texel_type const MaxTexel = base_type::nearest(Texture, Fetch, SampleCoordWrap, Layer, Face, ceil(Level), BorderColor);
			return mix(MinTexel, MaxTexel, fract(Level));
		}

		static texel_type nearest_mipmap_nearest
		(
			texture_type const & Texture, fetch_type Fetch, samplecoord_type const & SampleCoordWrap,
			typename texture_type::size_type Layer, typename texture_type::size_type Face, interpolate_type Level,
			texel_type const & BorderColor
		)
		{
			return base_type::nearest(Texture, Fetch, SampleCoordWrap, Layer, Face, round(Level), BorderColor);
		}
	};

	template <typename texture_type, typename interpolate_type, typename samplecoord_type, typename fetch_type, typename texel_type>
	struct filter3D<texture_type, interpolate_type, samplecoord_type, fetch_type, texel_type, false> : public filter_base<texture_type, interpolate_type, samplecoord_type, fetch_type, texel_type>
	{
		typedef filter_base<texture_type, interpolate_type, samplecoord_type, fetch_type, texel_type> base_type;

		static texel_type linear
		(
			texture_type const & Texture, fetch_type Fetch, samplecoord_type const & SampleCoord,
			typename texture_type::size_type Layer, typename texture_type::size_type Face, interpolate_type Level,
			texel_type const & BorderColor
		)
		{
			return base_type::nearest(Texture, Fetch, SampleCoord, Layer, Face, Level, BorderColor);
		}

		static texel_type linear_mipmap_linear
		(
			texture_type const & Texture, fetch_type Fetch, samplecoord_type const & SampleCoordWrap,
			typename texture_type::size_type Layer, typename texture_type::size_type Face, interpolate_type Level,
			texel_type const & BorderColor
		)
		{
			return base_type::nearest(Texture, Fetch, SampleCoordWrap, Layer, Face, round(Level), BorderColor);
		}

		static texel_type linear_mipmap_nearest
		(
			texture_type const & Texture, fetch_type Fetch, samplecoord_type const & SampleCoordWrap,
			typename texture_type::size_type Layer, typename texture_type::size_type Face, interpolate_type Level,
			texel_type const & BorderColor
		)
		{
			return base_type::nearest(Texture, Fetch, SampleCoordWrap, Layer, Face, round(Level), BorderColor);
		}

		static texel_type nearest_mipmap_linear
		(
			texture_type const & Texture, fetch_type Fetch, samplecoord_type const & SampleCoordWrap,
			typename texture_type::size_type Layer, typename texture_type::size_type Face, interpolate_type Level,
			texel_type const & BorderColor
		)
		{
			return base_type::nearest(Texture, Fetch, SampleCoordWrap, Layer, Face, round(Level), BorderColor);
		}

		static texel_type nearest_mipmap_nearest
		(
			texture_type const & Texture, fetch_type Fetch, samplecoord_type const & SampleCoordWrap,
			typename texture_type::size_type Layer, typename texture_type::size_type Face, interpolate_type Level,
			texel_type const & BorderColor
		)
		{
			return base_type::nearest(Texture, Fetch, SampleCoordWrap, Layer, Face, round(Level), BorderColor);
		}
	};
}//namespace detail
}//namespace gli

