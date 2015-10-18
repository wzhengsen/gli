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

#include "filter.hpp"

namespace gli{
namespace detail
{
	template <typename texture_type, typename fetch_type, typename texel_type>
	static texel_type filter2D_nearest
	(
		texture_type const & Texture,
		fetch_type Fetch,
		typename texture_type::samplecoord_type const & SampleCoord,
		typename texture_type::size_type Layer,
		typename texture_type::size_type Face,
		typename texture_type::size_type Level,
		texel_type const & BorderColor
	)
	{
		coord_nearest<typename texture_type::texelcoord_type, typename texture_type::samplecoord_type> const & Coord = make_coord_nearest(Texture.dimensions(Level), SampleCoord);

		texel_type Texel(BorderColor);
		if(Coord.UseTexel.s && Coord.UseTexel.t)
			Texel = Fetch(Texture, Coord.Texel, Layer, Face, Level);

		return Texel;
	}

	template <typename texture_type, typename fetch_type, typename texel_type>
	static texel_type filter2D_linear
	(
		texture_type const & Texture,
		fetch_type Fetch,
		typename texture_type::samplecoord_type const & SampleCoord,
		typename texture_type::size_type Layer,
		typename texture_type::size_type Face,
		typename texture_type::size_type Level,
		texel_type const & BorderColor
	)
	{
		coord_linear<typename texture_type::texelcoord_type, typename texture_type::samplecoord_type> const & Coord = make_coord_linear(Texture.dimensions(Level), SampleCoord);

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
}//namespace detail
}//namespace gli

