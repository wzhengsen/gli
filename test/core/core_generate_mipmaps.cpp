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
/// @file gli/core/core_generate_mipmaps.cpp
/// @date 2015-10-05 / 2015-10-05
/// @author Christophe Riccio
///////////////////////////////////////////////////////////////////////////////////

#include <gli/comparison.hpp>
#include <gli/type.hpp>
#include <gli/view.hpp>
#include <gli/copy.hpp>
#include <gli/generate_mipmaps.hpp>

#include <glm/gtc/epsilon.hpp>

namespace detail
{
	template <typename retType, typename T, gli::precision P, template <typename, gli::precision> class vecType, bool normalize = false, bool srgb = false>
	struct convert
	{
		static vecType<retType, P> fetch(gli::texture2D const & Texture, gli::texture2D::dim_type const & TexelCoord, gli::texture2D::size_type Level)
		{
			return vecType<retType, P>(Texture.load<vecType<T, P> >(TexelCoord, Level));
		}

		static void write(gli::texture2D & Texture, gli::texture2D::dim_type const & TexelCoord, gli::texture2D::size_type Level, vecType<retType, P> const & Texel)
		{
			Texture.store<vecType<T, P> >(TexelCoord, Level, vecType<T, P>(Texel));
		}
	};

	template <typename retType, typename T, gli::precision P, template <typename, gli::precision> class vecType>
	struct convert<retType, T, P, vecType, true, false>
	{
		static vecType<retType, P> fetch(gli::texture2D const & Texture, gli::texture2D::dim_type const & TexelCoord, gli::texture2D::size_type Level)
		{
			return gli::compNormalize<retType>(Texture.load<vecType<T, P> >(TexelCoord, Level));
		}

		static void write(gli::texture2D & Texture, gli::texture2D::dim_type const & TexelCoord, gli::texture2D::size_type Level, vecType<retType, P> const & Texel)
		{
			Texture.store<vecType<T, P> >(TexelCoord, Level, glm::compScale<T>(Texel));
		}
	};

	template <typename retType, typename T, gli::precision P, template <typename, gli::precision> class vecType>
	struct convert<retType, T, P, vecType, true, true>
	{
		static vecType<retType, P> fetch(gli::texture2D const & Texture, gli::texture2D::dim_type const & TexelCoord, gli::texture2D::size_type Level)
		{
			return gli::convertSRGBToLinear(gli::compNormalize<retType>(Texture.load<vecType<T, P> >(TexelCoord, Level)));
		}

		static void write(gli::texture2D & Texture, gli::texture2D::dim_type const & TexelCoord, gli::texture2D::size_type Level, vecType<retType, P> const & Texel)
		{
			Texture.store<vecType<T, P> >(TexelCoord, Level, gli::compScale<T>(gli::convertLinearToSRGB(Texel)));
		}
	};
}//namespace detail

namespace texel
{
	int test()
	{
		int Error = 0;

		{
			gli::texture2D Texture(gli::FORMAT_RGBA8_UINT, gli::texture2D::dim_type(1), 1);
			detail::convert<glm::u8, glm::u8, gli::defaultp, gli::tvec4, false, false>::write(Texture, gli::texture2D::dim_type(0), 0, gli::u8vec4(255, 127, 0, 255));
			glm::u8vec4 Texel = detail::convert<glm::u8, glm::u8, gli::defaultp, gli::tvec4, false, false>::fetch(Texture, gli::texture2D::dim_type(0), 0);

			Error += Texel == gli::u8vec4(255, 127, 0, 255) ? 0 : 1;
		}

		{
			gli::texture2D Texture(gli::FORMAT_RGBA8_UNORM, gli::texture2D::dim_type(1), 1);
			detail::convert<float, glm::u8, gli::defaultp, gli::tvec4, true, false>::write(Texture, gli::texture2D::dim_type(0), 0, gli::vec4(1.0f, 0.5f, 0.0f, 1.0f));
			glm::vec4 Texel = detail::convert<float, glm::u8, gli::defaultp, gli::tvec4, true, false>::fetch(Texture, gli::texture2D::dim_type(0), 0);

			Error += glm::all(glm::epsilonEqual(Texel, gli::vec4(1.0f, 0.5f, 0.0f, 1.0f), 0.01f)) ? 0 : 1;
		}

		{
			gli::texture2D Texture(gli::FORMAT_RGBA8_SRGB, gli::texture2D::dim_type(1), 1);
			detail::convert<float, glm::u8, gli::defaultp, gli::tvec4, true, true>::write(Texture, gli::texture2D::dim_type(0), 0, gli::vec4(1.0f, 0.5f, 0.0f, 1.0f));
			glm::vec4 Texel = detail::convert<float, glm::u8, gli::defaultp, gli::tvec4, true, true>::fetch(Texture, gli::texture2D::dim_type(0), 0);

			Error += glm::all(glm::epsilonEqual(Texel, gli::vec4(1.0f, 0.5f, 0.0f, 1.0f), 0.01f)) ? 0 : 1;
		}

		return Error;
	}
}//namespace texel

namespace mipmaps
{
	int test()
	{
		int Error = 0;

		gli::texture2D Texture(gli::FORMAT_RGBA8_UNORM, gli::texture2D::dim_type(64));
		for(std::size_t j = 0, n = Texture.dimensions().y; j < n; ++j)
		for(std::size_t i = 0, m = Texture.dimensions().x; i < n; ++i)
			Texture.store<gli::u8vec4>(gli::dim2_t(i , j), 0, gli::u8vec4(i * 4, j * 4, 0, 255));

		gli::texture2D TextureView(gli::view(Texture, 0, 0));

		// Custom mipmaps generation using a sampler object
		gli::fsampler2D SamplerA(gli::texture2D(gli::copy(Texture)), gli::WRAP_CLAMP_TO_EDGE, gli::FILTER_LINEAR, gli::FILTER_LINEAR);
		SamplerA.generate_mipmaps();

		gli::texture2D MipmapsA = SamplerA();
		gli::texture2D MipmapViewA(gli::view(MipmapsA, 0, 0));

		Error += TextureView == MipmapViewA ? 0 : 1;
/*
		// Mipmaps generation using the wrapper function
		gli::texture2D MipmapsB = gli::generate_mipmaps(gli::texture2D(gli::copy(Texture)));
		gli::texture2D MipmapViewB(gli::view(MipmapsB, 0, 0));

		Error += TextureView == MipmapViewB ? 0 : 1;

		// Compare custom mipmaps generation and wrapper mipmaps generation
		Error += MipmapViewA == MipmapViewB ? 0 : 1;
		Error += MipmapsA == MipmapsB ? 0 : 1;
*/
		return Error;
	}
}//namespace mipmaps

int main()
{
	int Error = 0;

	Error += mipmaps::test();
	Error += texel::test();

	return Error;
}

