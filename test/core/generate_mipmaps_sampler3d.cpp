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
/// @file gli/test/core/generate_mipmaps_sampler3d.cpp
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

		glm::uint16 const Black(gli::packUnorm4x4(glm::vec4(0.0f, 0.0f, 0.0f, 0.0f)));
		glm::uint16 const Color(gli::packUnorm4x4(glm::vec4(1.0f, 0.5f, 0.0f, 1.0f)));

		gli::texture3D Texture(gli::FORMAT_RGBA4_UNORM, gli::texture3D::texelcoord_type(32));
		Texture.clear(Black);
		Texture[0].clear(Color);

		glm::uint16 const LoadC = Texture.load<glm::uint16>(gli::texture3D::texelcoord_type(0), Texture.max_level());
		Error += LoadC == Black ? 0 : 1;

		gli::texture3D TextureView(gli::view(Texture, 0, 0));

		// Custom mipmaps generation using a sampler object
		gli::fsampler3D SamplerA(gli::texture3D(gli::copy(Texture)), gli::WRAP_CLAMP_TO_EDGE);
		SamplerA.generate_mipmaps(gli::FILTER_LINEAR);

		gli::texture3D MipmapsA = SamplerA();
		gli::uint16 const LoadA = MipmapsA.load<gli::uint16>(gli::texture3D::texelcoord_type(0), MipmapsA.max_level());
		Error += LoadA == Color ? 0 : 1;
		Error += LoadA != LoadC ? 0 : 1;

		gli::texture3D MipmapViewA(gli::view(MipmapsA, 0, 0));
		Error += TextureView == MipmapViewA ? 0 : 1;

		// Mipmaps generation using the wrapper function
		gli::texture3D MipmapsB = gli::generate_mipmaps(gli::texture3D(gli::copy(Texture)), gli::FILTER_LINEAR);
		glm::uint16 const LoadB = MipmapsB.load<glm::uint16>(gli::texture3D::texelcoord_type(0), MipmapsB.max_level());
		Error += LoadB == Color ? 0 : 1;
		Error += LoadB != LoadC ? 0 : 1;

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

		glm::vec4 const Black(0.0f, 0.0f, 0.0f, 0.0f);
		glm::vec4 const Color(1.0f, 0.5f, 0.0f, 1.0f);

		gli::texture3D Texture(gli::FORMAT_RGBA32_SFLOAT, gli::texture3D::texelcoord_type(32));
		Texture.clear(Black);
		Texture[0].clear(Color);

		glm::vec4 const LoadC = Texture.load<glm::vec4>(gli::texture3D::texelcoord_type(0), Texture.max_level());
		Error += LoadC == Black ? 0 : 1;

		gli::texture3D TextureView(gli::view(Texture, 0, 0));

		// Custom mipmaps generation using a sampler object
		gli::fsampler3D SamplerA(gli::texture3D(gli::copy(Texture)), gli::WRAP_CLAMP_TO_EDGE);
		SamplerA.generate_mipmaps(gli::FILTER_LINEAR);

		gli::texture3D MipmapsA = SamplerA();
		gli::vec4 const LoadA = MipmapsA.load<gli::vec4>(gli::texture3D::texelcoord_type(0), MipmapsA.max_level());
		Error += LoadA == Color ? 0 : 1;
		Error += LoadA != LoadC ? 0 : 1;

		gli::texture3D MipmapViewA(gli::view(MipmapsA, 0, 0));
		Error += TextureView == MipmapViewA ? 0 : 1;

		// Mipmaps generation using the wrapper function
		gli::texture3D MipmapsB = gli::generate_mipmaps(gli::texture3D(gli::copy(Texture)), gli::FILTER_LINEAR);
		glm::vec4 const LoadB = MipmapsB.load<glm::vec4>(gli::texture3D::texelcoord_type(0), MipmapsB.max_level());
		Error += LoadB == Color ? 0 : 1;
		Error += LoadB != LoadC ? 0 : 1;

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

		glm::u8vec4 const Black(0, 0, 0, 0);
		glm::u8vec4 const Color(255, 127, 0, 255);

		gli::texture3D Texture(gli::FORMAT_RGBA8_UNORM, gli::texture3D::texelcoord_type(32));
		Texture.clear(Black);
		Texture[0].clear(Color);

		glm::u8vec4 const LoadC = Texture.load<glm::u8vec4>(gli::texture3D::texelcoord_type(0), Texture.max_level());
		Error += LoadC == Black ? 0 : 1;

		gli::texture3D TextureView(gli::view(Texture, 0, 0));

		// Custom mipmaps generation using a sampler object
		gli::fsampler3D SamplerA(gli::texture3D(gli::copy(Texture)), gli::WRAP_CLAMP_TO_EDGE);
		SamplerA.generate_mipmaps(gli::FILTER_LINEAR);

		gli::texture3D MipmapsA = SamplerA();
		glm::u8vec4 const LoadA = MipmapsA.load<glm::u8vec4>(gli::texture3D::texelcoord_type(0), MipmapsA.max_level());
		Error += LoadA == Color ? 0 : 1;
		Error += LoadA != LoadC ? 0 : 1;

		gli::texture3D MipmapViewA(gli::view(MipmapsA, 0, 0));
		Error += TextureView == MipmapViewA ? 0 : 1;

		// Mipmaps generation using the wrapper function
		gli::texture3D MipmapsB = gli::generate_mipmaps(gli::texture3D(gli::copy(Texture)), gli::FILTER_LINEAR);
		glm::u8vec4 const LoadB = MipmapsB.load<glm::u8vec4>(gli::texture3D::texelcoord_type(0), MipmapsB.max_level());
		Error += LoadB == Color ? 0 : 1;
		Error += LoadB != LoadC ? 0 : 1;

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

		glm::i8vec4 const Black(0, 0, 0, 0);
		glm::i8vec4 const Color(127, 63, 0, 1);

		gli::texture3D Texture(gli::FORMAT_RGBA8_SNORM, gli::texture3D::texelcoord_type(32));
		Texture.clear(Black);
		Texture[0].clear(Color);

		glm::i8vec4 const LoadC = Texture.load<glm::i8vec4>(gli::texture3D::texelcoord_type(0), Texture.max_level());
		Error += LoadC == Black ? 0 : 1;

		gli::texture3D TextureView(gli::view(Texture, 0, 0));

		// Custom mipmaps generation using a sampler object
		gli::fsampler3D SamplerA(gli::texture3D(gli::copy(Texture)), gli::WRAP_CLAMP_TO_EDGE);
		SamplerA.generate_mipmaps(gli::FILTER_LINEAR);

		gli::texture3D MipmapsA = SamplerA();
		glm::i8vec4 const LoadA = MipmapsA.load<glm::i8vec4>(gli::texture3D::texelcoord_type(0), MipmapsA.max_level());
		Error += LoadA == Color ? 0 : 1;
		Error += LoadA != LoadC ? 0 : 1;

		gli::texture3D MipmapViewA(gli::view(MipmapsA, 0, 0));
		Error += TextureView == MipmapViewA ? 0 : 1;

		// Mipmaps generation using the wrapper function
		gli::texture3D MipmapsB = gli::generate_mipmaps(gli::texture3D(gli::copy(Texture)), gli::FILTER_LINEAR);
		glm::i8vec4 const LoadB = MipmapsB.load<glm::i8vec4>(gli::texture3D::texelcoord_type(0), MipmapsB.max_level());
		Error += LoadB == Color ? 0 : 1;
		Error += LoadB != LoadC ? 0 : 1;

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

		glm::uint32 const Black = gli::packUnorm3x10_1x2(glm::vec4(0.0f, 0.0f, 0.0f, 0.0f));
		gli::uint32 const Color = gli::packUnorm3x10_1x2(glm::vec4(1.0f, 0.5f, 0.0f, 1.0f));

		gli::texture3D Texture(gli::FORMAT_RGB10A2_UNORM, gli::texture3D::texelcoord_type(32));
		Texture.clear(Black);
		Texture[0].clear(Color);

		glm::uint32 const LoadC = Texture.load<glm::uint32>(gli::texture3D::texelcoord_type(0), Texture.max_level());
		Error += LoadC == Black ? 0 : 1;

		gli::texture3D TextureView(gli::view(Texture, 0, 0));
		gli::fsampler3D SamplerA(gli::texture3D(gli::copy(Texture)), gli::WRAP_CLAMP_TO_EDGE);
		SamplerA.generate_mipmaps(gli::FILTER_LINEAR);

		gli::texture3D MipmapsA = SamplerA();
		gli::uint32 const LoadA = MipmapsA.load<gli::uint32>(gli::texture3D::texelcoord_type(0), MipmapsA.max_level());
		Error += LoadA == Color ? 0 : 1;
		Error += LoadA != LoadC ? 0 : 1;

		gli::texture3D MipmapViewA(gli::view(MipmapsA, 0, 0));
		Error += TextureView == MipmapViewA ? 0 : 1;

		// Mipmaps generation using the wrapper function
		gli::texture3D MipmapsB = gli::generate_mipmaps(gli::texture3D(gli::copy(Texture)), gli::FILTER_LINEAR);
		gli::uint32 const LoadB = MipmapsB.load<gli::uint32>(gli::texture3D::texelcoord_type(0), MipmapsB.max_level());
		Error += LoadB == Color ? 0 : 1;
		Error += LoadB != LoadC ? 0 : 1;

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

	Error += mipmaps_rgba4unorm::test();
	Error += mipmaps_rgba8unorm::test();
	Error += mipmaps_rgba32sf::test();
	Error += mipmaps_rgba8snorm::test();
	Error += mipmaps_rgb10a2unorm::test();

	return Error;
}

