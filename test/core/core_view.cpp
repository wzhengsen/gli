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

	std::vector<glm::u8vec4> Color(6);
	Color.push_back(glm::u8vec4(255,   0,   0, 255));
	Color.push_back(glm::u8vec4(255, 127,   0, 255));
	Color.push_back(glm::u8vec4(255, 255,   0, 255));
	Color.push_back(glm::u8vec4(  0, 255,   0, 255));
	Color.push_back(glm::u8vec4(  0, 255, 255, 255));
	Color.push_back(glm::u8vec4(  0,   0, 255, 255));

	gli::texture2D TextureA(
		glm::log2(32) + 1, 
		gli::RGBA8_UNORM, 
		gli::texture2D::dimensions_type(32));

	for(gli::texture2D::size_type Level = 0; Level < TextureA.levels(); ++Level)
	for(gli::texture2D::size_type Texel = 0; Texel < TextureA[Level].size<glm::u8vec4>(); ++Texel)
		*(TextureA[Level].data<glm::u8vec4>() + Texel) = Color[Level];

	gli::texture2D::size_type const TextureASize = TextureA.size();

	gli::texture2D TextureViewA = gli::view2D(
		TextureA, 
		TextureA.format(), 
		TextureA.view().BaseLayer, 
		TextureA.view().BaseFace, 
		TextureA.view().BaseLevel, 
		TextureA.view().MaxLevel);

	gli::texture2D::size_type const TextureViewASize = TextureViewA.size();

	Error += TextureASize == TextureViewASize ? 0 : 1;

	gli::texture2D TextureB(
		glm::log2(16) + 1, 
		gli::RGBA8_UNORM, 
		gli::texture2D::dimensions_type(16));

	for(gli::texture2D::size_type Level = 0; Level < TextureB.levels(); ++Level)
	for(gli::texture2D::size_type Texel = 0; Texel < TextureB[Level].size<glm::u8vec4>(); ++Texel)
		*(TextureB[Level].data<glm::u8vec4>() + Texel) = Color[Level + 1];

	gli::texture2D::size_type const TextureBSize = TextureB.size();

	gli::texture2D TextureViewB = gli::view2D(
		TextureA, 
		TextureA.format(), 
		TextureA.view().BaseLayer, 
		TextureA.view().BaseFace, 
		TextureA.view().BaseLevel + 1, 
		TextureA.view().MaxLevel);

	gli::texture2D::size_type const TextureViewBSize = TextureViewB.size();

	Error += TextureASize > TextureViewBSize ? 0 : 1;
	Error += TextureBSize == TextureViewBSize ? 0 : 1;

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
