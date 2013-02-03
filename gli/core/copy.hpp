///////////////////////////////////////////////////////////////////////////////////
/// OpenGL Image (gli.g-truc.net)
///
/// Copyright (c) 2008 - 2013 G-Truc Creation (www.g-truc.net)
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
/// @file gli/core/copy.hpp
/// @date 2013-02-01 / 2013-02-03
/// @author Christophe Riccio
///////////////////////////////////////////////////////////////////////////////////

#ifndef GLI_CORE_COPY_INCLUDED
#define GLI_CORE_COPY_INCLUDED

#include "texture1d.hpp"
#include "texture1d_array.hpp"
#include "texture2d.hpp"
#include "texture2d_array.hpp"
#include "texture3d.hpp"
#include "texture_cube.hpp"
#include "texture_cube_array.hpp"

namespace gli
{
	image copy(image const & Image);

	template <typename texture>
	texture copy(texture const & Texture);

/*
	void copy(
		texture2D const & srcTexture,
		texture2D::size_type const & srcLevel,
		texture2D::dimensions_type const & srcPosition,
		texture2D & dstTexture,
		texture2D::size_type const & dstLevel,
		texture2D::dimensions_type const & dstPosition,
		texture2D::dimensions_type const & dimentions);
*/
}//namespace gli

#include "copy.inl"

#endif//GLI_CORE_COPY_INCLUDED

/*
void glCopyImageSubData(	GLuint  	srcName,
						GLenum  	srcTarget,
						GLint  	srcLevel,
						GLint  	srcX,
						GLint  	srcY,
						GLint  	srcZ,
						GLuint  	dstName,
						GLenum  	dstTarget,
						GLint  	dstLevel,
						GLint  	dstX,
						GLint  	dstY,
						GLint  	dstZ,
						GLsizei  	srcWidth,
						GLsizei  	srcHeight,
						GLsizei  	srcDepth);
*/
