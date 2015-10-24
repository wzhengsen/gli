//////////////////////////////////////////////////////////////////////////////////
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
/// @file gli/core/core_sampler3d_generate_mipmaps.cpp
/// @date 2015-10-05 / 2015-10-05
/// @author Christophe Riccio
///////////////////////////////////////////////////////////////////////////////////

#include <gli/comparison.hpp>
#include <gli/type.hpp>
#include <gli/view.hpp>
#include <gli/copy.hpp>
#include <gli/generate_mipmaps.hpp>

#include <glm/gtc/epsilon.hpp>

namespace mipmaps_rgba4unorm
{
	int test()
	{
		int Error = 0;

		gli::texture3D Texture(gli::FORMAT_RGBA4_UNORM, gli::texture3D::texelcoord_type(32));
		Texture.clear(gli::packUnorm4x4(glm::vec4(1.0f, 0.5f, 0.0f, 1.0f)));

		gli::texture3D TextureView(gli::view(Texture, 0, 0));

		// Custom mipmaps generation using a sampler object
		gli::fsampler3D SamplerA(gli::texture3D(gli::copy(Texture)), gli::WRAP_CLAMP_TO_EDGE, gli::FILTER_LINEAR, gli::FILTER_LINEAR);
		SamplerA.generate_mipmaps();

		gli::texture3D MipmapsA = SamplerA();
		gli::texture3D MipmapViewA(gli::view(MipmapsA, 0, 0));

		Error += TextureView == MipmapViewA ? 0 : 1;

		// Mipmaps generation using the wrapper function
		gli::texture3D MipmapsB = gli::generate_mipmaps(gli::texture3D(gli::copy(Texture)));
		gli::texture3D MipmapViewB(gli::view(MipmapsB, 0, 0));

		Error += TextureView == MipmapViewB ? 0 : 1;

		// Compare custom mipmaps generation and wrapper mipmaps generation
		Error += MipmapViewA == MipmapViewB ? 0 : 1;
		Error += MipmapsA == MipmapsB ? 0 : 1;

		return Error;
	}
}//namespace mipmaps_rgba4unorm

namespace mipmaps_rgba32sf
{
	int test()
	{
		int Error = 0;

		gli::texture3D Texture(gli::FORMAT_RGBA32_SFLOAT, gli::texture3D::texelcoord_type(256));
		Texture.clear(glm::vec4(1.0f, 0.5f, 0.0f, 1.0f));

		gli::texture3D TextureView(gli::view(Texture, 0, 0));

		// Custom mipmaps generation using a sampler object
		gli::fsampler3D SamplerA(gli::texture3D(gli::copy(Texture)), gli::WRAP_CLAMP_TO_EDGE, gli::FILTER_LINEAR, gli::FILTER_LINEAR);
		SamplerA.generate_mipmaps();

		gli::texture3D MipmapsA = SamplerA();
		gli::texture3D MipmapViewA(gli::view(MipmapsA, 0, 0));

		Error += TextureView == MipmapViewA ? 0 : 1;

		// Mipmaps generation using the wrapper function
		gli::texture3D MipmapsB = gli::generate_mipmaps(gli::texture3D(gli::copy(Texture)));
		gli::texture3D MipmapViewB(gli::view(MipmapsB, 0, 0));

		Error += TextureView == MipmapViewB ? 0 : 1;

		// Compare custom mipmaps generation and wrapper mipmaps generation
		Error += MipmapViewA == MipmapViewB ? 0 : 1;
		Error += MipmapsA == MipmapsB ? 0 : 1;

		return Error;
	}
}//namespace mipmaps_rgba32sf

namespace mipmaps_rgba8unorm
{
	int test()
	{
		int Error = 0;

		gli::texture3D Texture(gli::FORMAT_RGBA8_UNORM, gli::texture3D::texelcoord_type(128));
		Texture.clear(glm::u8vec4(255, 127, 0, 255));

		gli::texture3D TextureView(gli::view(Texture, 0, 0));

		// Custom mipmaps generation using a sampler object
		gli::fsampler3D SamplerA(gli::texture3D(gli::copy(Texture)), gli::WRAP_CLAMP_TO_EDGE, gli::FILTER_LINEAR, gli::FILTER_LINEAR);
		SamplerA.generate_mipmaps();

		gli::texture3D MipmapsA = SamplerA();
		gli::texture3D MipmapViewA(gli::view(MipmapsA, 0, 0));

		Error += TextureView == MipmapViewA ? 0 : 1;

		// Mipmaps generation using the wrapper function
		gli::texture3D MipmapsB = gli::generate_mipmaps(gli::texture3D(gli::copy(Texture)));
		gli::texture3D MipmapViewB(gli::view(MipmapsB, 0, 0));

		Error += TextureView == MipmapViewB ? 0 : 1;

		// Compare custom mipmaps generation and wrapper mipmaps generation
		Error += MipmapViewA == MipmapViewB ? 0 : 1;
		Error += MipmapsA == MipmapsB ? 0 : 1;

		return Error;
	}
}//namespace mipmaps_rgba8unorm

namespace mipmaps_rgba8snorm
{
	int test()
	{
		int Error = 0;

		gli::texture3D Texture(gli::FORMAT_RGBA8_SNORM, gli::texture3D::texelcoord_type(32));
		Texture.clear(glm::i8vec4(127, 63, 0, 1));

		gli::texture3D TextureView(gli::view(Texture, 0, 0));

		// Custom mipmaps generation using a sampler object
		gli::fsampler3D SamplerA(gli::texture3D(gli::copy(Texture)), gli::WRAP_CLAMP_TO_EDGE, gli::FILTER_LINEAR, gli::FILTER_LINEAR);
		SamplerA.generate_mipmaps();

		gli::texture3D MipmapsA = SamplerA();
		gli::texture3D MipmapViewA(gli::view(MipmapsA, 0, 0));

		Error += TextureView == MipmapViewA ? 0 : 1;

		// Mipmaps generation using the wrapper function
		gli::texture3D MipmapsB = gli::generate_mipmaps(gli::texture3D(gli::copy(Texture)));
		gli::texture3D MipmapViewB(gli::view(MipmapsB, 0, 0));

		Error += TextureView == MipmapViewB ? 0 : 1;

		// Compare custom mipmaps generation and wrapper mipmaps generation
		Error += MipmapViewA == MipmapViewB ? 0 : 1;
		Error += MipmapsA == MipmapsB ? 0 : 1;

		return Error;
	}
}//namespace mipmaps_rgba8snorm

namespace mipmaps_rgb10a2unorm
{
	int test()
	{
		int Error = 0;

		gli::texture3D Texture(gli::FORMAT_RGB10A2_UNORM, gli::texture3D::texelcoord_type(32));
		Texture.clear(gli::packUnorm3x10_1x2(glm::vec4(1.0f, 0.5f, 0.0f, 1.0f)));

		gli::texture3D TextureView(gli::view(Texture, 0, 0));

		// Custom mipmaps generation using a sampler object
		gli::fsampler3D SamplerA(gli::texture3D(gli::copy(Texture)), gli::WRAP_CLAMP_TO_EDGE, gli::FILTER_LINEAR, gli::FILTER_LINEAR);
		SamplerA.generate_mipmaps();

		gli::texture3D MipmapsA = SamplerA();
		gli::texture3D MipmapViewA(gli::view(MipmapsA, 0, 0));

		Error += TextureView == MipmapViewA ? 0 : 1;

		// Mipmaps generation using the wrapper function
		gli::texture3D MipmapsB = gli::generate_mipmaps(gli::texture3D(gli::copy(Texture)));
		gli::texture3D MipmapViewB(gli::view(MipmapsB, 0, 0));

		Error += TextureView == MipmapViewB ? 0 : 1;

		// Compare custom mipmaps generation and wrapper mipmaps generation
		Error += MipmapViewA == MipmapViewB ? 0 : 1;
		Error += MipmapsA == MipmapsB ? 0 : 1;

		return Error;
	}
}//namespace mipmaps_rgb10a2unorm

int main()
{
	int Error = 0;

//	Error += mipmaps_rgba4unorm::test();
//	Error += mipmaps_rgba8unorm::test();
	Error += mipmaps_rgba32sf::test();
//	Error += mipmaps_rgba8snorm::test();
//	Error += mipmaps_rgb10a2unorm::test();

	return Error;
}

