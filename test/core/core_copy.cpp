///////////////////////////////////////////////////////////////////////////////////////////////////
// OpenGL Image Copyright (c) 2008 - 2013 G-Truc Creation (www.g-truc.net)
///////////////////////////////////////////////////////////////////////////////////////////////////
// Created : 2013-02-04
// Updated : 2013-02-04
// Licence : This source is under MIT licence
// File    : test/core/core_copy.cpp
///////////////////////////////////////////////////////////////////////////////////////////////////

#include <gli/gli.hpp>

int test_texture1D
(
	std::vector<gli::format> const & Formats, 
	gli::texture1D::size_type const & TextureSize
)
{
	int Error(0);

	for(std::size_t i = 0; i < Formats.size(); ++i)
	{
		gli::texture1D TextureA(
			gli::texture1D::size_type(glm::log2(float(TextureSize))) + 1,
			Formats[i],
			gli::texture1D::dimensions_type(TextureSize));

		gli::texture1D TextureB = gli::copy(TextureA);

		Error += TextureA == TextureB ? 0 : 1;

		gli::texture1D TextureC(TextureA, 
			gli::texture1D::size_type(1), gli::texture1D::size_type(2));

		Error += TextureA[1] == TextureC[0] ? 0 : 1;
		Error += TextureA[2] == TextureC[1] ? 0 : 1;

		gli::texture1D TextureD = gli::copy(TextureC);

		Error += TextureC == TextureD ? 0 : 1;
	}

	return Error;
}

int test_texture2D
(
	std::vector<gli::format> const & Formats, 
	gli::texture2D::size_type const & TextureSize
)
{
	int Error(0);

	for(std::size_t i = 0; i < Formats.size(); ++i)
	{
		gli::texture2D TextureA(
			gli::texture2D::size_type(glm::log2(float(TextureSize))) + 1,
			Formats[i],
			gli::texture2D::dimensions_type(TextureSize));

		gli::texture2D TextureB = gli::copy(TextureA);

		Error += TextureA == TextureB ? 0 : 1;

		gli::texture2D TextureC(TextureA, 
			gli::texture2D::size_type(1), gli::texture2D::size_type(2));

		Error += TextureA[1] == TextureC[0] ? 0 : 1;
		Error += TextureA[2] == TextureC[1] ? 0 : 1;

		gli::texture2D TextureD = gli::copy(TextureC);

		Error += TextureC == TextureD ? 0 : 1;
	}

	return Error;
}

int test_texture3D
(
	std::vector<gli::format> const & Formats, 
	gli::texture3D::size_type const & TextureSize
)
{
	int Error(0);

	for(std::size_t i = 0; i < Formats.size(); ++i)
	{
		gli::texture3D TextureA(
			gli::texture3D::size_type(glm::log2(float(TextureSize))) + 1,
			Formats[i],
			gli::texture3D::dimensions_type(TextureSize));

		gli::texture3D TextureB = gli::copy(TextureA);

		Error += TextureA == TextureB ? 0 : 1;

		gli::texture3D TextureC(TextureA, 
			gli::texture3D::size_type(1), gli::texture3D::size_type(2));

		Error += TextureA[1] == TextureC[0] ? 0 : 1;
		Error += TextureA[2] == TextureC[1] ? 0 : 1;

		gli::texture3D TextureD = gli::copy(TextureC);

		Error += TextureC == TextureD ? 0 : 1;
	}

	return Error;
}

int main()
{
	int Error(0);

	std::vector<gli::format> Formats;
	Formats.push_back(gli::RGBA8_UNORM);
	Formats.push_back(gli::RGB8_UNORM);
	Formats.push_back(gli::RGB_DXT1);
	Formats.push_back(gli::RGB_BP_UNORM);
	Formats.push_back(gli::RGBA32F);
	std::size_t const TextureSize = 32;

	Error += test_texture1D(Formats, TextureSize);
	Error += test_texture2D(Formats, TextureSize);
	Error += test_texture3D(Formats, TextureSize);
		
	return Error;
}
