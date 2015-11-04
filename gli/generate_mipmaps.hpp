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
/// @file gli/generate_mipmaps.hpp
/// @date 2015-10-06 / 2015-10-06
/// @author Christophe Riccio
///////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "texture1d.hpp"
#include "texture2d.hpp"
#include "texture3d.hpp"
#include "filter.hpp"

namespace gli
{
	/// 
	texture1D generate_mipmaps(texture1D const & Texture, texture1D::size_type BaseLevel, texture1D::size_type MaxLevel, filter Minification);

	/// 
	texture1D generate_mipmaps(texture1D const & Texture, filter Minification);

	/// 
	texture2D generate_mipmaps(texture2D const & Texture, texture2D::size_type BaseLevel, texture2D::size_type MaxLevel, filter Minification);

	/// 
	texture2D generate_mipmaps(texture2D const & Texture, filter Minification);

	/// 
	texture3D generate_mipmaps(texture3D const & Texture, texture3D::size_type BaseLevel, texture3D::size_type MaxLevel, filter Minification);

	/// 
	texture3D generate_mipmaps(texture3D const & Texture, filter Minification);
}//namespace gli

#include "./core/generate_mipmaps.inl"
