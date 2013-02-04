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
/// @file gli/core/core_view.cpp
/// @date 2013-02-03 / 2013-02-03
/// @author Christophe Riccio
///////////////////////////////////////////////////////////////////////////////////

#include <gli/gli.hpp>

int test_view1D()
{
	int Error(0);

	return Error;
}

int test_view1DArray()
{
	int Error(0);

	return Error;
}

int test_view2D()
{
	int Error(0);

	gli::texture2D Texture2D(
		glm::log2(8) + 1, 
		gli::RGBA8_UNORM, 
		gli::texture2D::dimensions_type(8));

	gli::texture2D::size_type const Texture2DSize = Texture2D.size();

	gli::texture2D TextureViewA = gli::view2D(
		Texture2D, 
		Texture2D.format(), 
		Texture2D.view().BaseLayer, 
		Texture2D.view().BaseFace, 
		Texture2D.view().BaseLevel, 
		Texture2D.view().MaxLevel);

	gli::texture2D::size_type const TextureViewASize = TextureViewA.size();

	Error += Texture2DSize == TextureViewASize ? 0 : 1;

	gli::texture2D TextureViewB = gli::view2D(
		Texture2D, 
		Texture2D.format(), 
		Texture2D.view().BaseLayer, 
		Texture2D.view().BaseFace, 
		Texture2D.view().BaseLevel + 1, 
		Texture2D.view().MaxLevel);

	gli::texture2D::size_type const TextureViewBSize = TextureViewB.size();

	Error += Texture2DSize > TextureViewBSize ? 0 : 1;

	return Error;
}

int test_view2DArray()
{
	int Error(0);

	return Error;
}

int test_view3D()
{
	int Error(0);

	return Error;
}

int test_viewCube()
{
	int Error(0);

	return Error;
}

int test_viewCubeArray()
{
	int Error(0);

	return Error;
}

int main()
{
	int Error(0);

	Error += test_view1D();
	Error += test_view1DArray();
	Error += test_view2D();
	Error += test_view2DArray();
	Error += test_view3D();
	Error += test_viewCube();
	Error += test_viewCubeArray();
		
	return Error;
}
