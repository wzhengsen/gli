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
/// @file gli/core/core_convert.cpp
/// @date 2015-10-05 / 2015-10-05
/// @author Christophe Riccio
///////////////////////////////////////////////////////////////////////////////////

#include <gli/sampler2d.hpp>
#include <glm/gtc/epsilon.hpp>

namespace rgba8_int
{
	int test()
	{
		int Error = 0;

		{
			gli::texture2D Texture(gli::FORMAT_RGBA8_UINT, gli::texture2D::dim_type(1), 1);
			gli::detail::convert<gli::texture2D, glm::u8, glm::u8, gli::defaultp, gli::tvec4, gli::detail::CONVERT_MODE_CAST>::write(Texture, gli::texture2D::dim_type(0), 0, gli::u8vec4(255, 127, 0, 255));
			glm::u8vec4 Texel = gli::detail::convert<gli::texture2D, glm::u8, glm::u8, gli::defaultp, gli::tvec4, gli::detail::CONVERT_MODE_CAST>::fetch(Texture, gli::texture2D::dim_type(0), 0);

			Error += Texel == gli::u8vec4(255, 127, 0, 255) ? 0 : 1;
		}

		{
			gli::texture2D Texture(gli::FORMAT_RGBA8_SINT, gli::texture2D::dim_type(1), 1);
			gli::detail::convert<gli::texture2D, glm::i8, glm::i8, gli::defaultp, gli::tvec4, gli::detail::CONVERT_MODE_CAST>::write(Texture, gli::texture2D::dim_type(0), 0, gli::i8vec4(127, 63, 0, 127));
			glm::i8vec4 Texel = gli::detail::convert<gli::texture2D, glm::i8, glm::i8, gli::defaultp, gli::tvec4, gli::detail::CONVERT_MODE_CAST>::fetch(Texture, gli::texture2D::dim_type(0), 0);

			Error += Texel == gli::i8vec4(127, 63, 0, 127) ? 0 : 1;
		}

		return Error;
	}
}//namespace rgba8_int

namespace rgba8_norm
{
	int test()
	{
		int Error = 0;

		{
			gli::texture2D Texture(gli::FORMAT_RGBA8_UNORM, gli::texture2D::dim_type(1), 1);
			gli::detail::convert<gli::texture2D, float, glm::u8, gli::defaultp, gli::tvec4, gli::detail::CONVERT_MODE_NORM>::write(Texture, gli::texture2D::dim_type(0), 0, gli::vec4(1.0f, 0.5f, 0.0f, 1.0f));
			gli::vec4 Texel = gli::detail::convert<gli::texture2D, float, glm::u8, gli::defaultp, gli::tvec4, gli::detail::CONVERT_MODE_NORM>::fetch(Texture, gli::texture2D::dim_type(0), 0);

			Error += gli::all(gli::epsilonEqual(Texel, gli::vec4(1.0f, 0.5f, 0.0f, 1.0f), 0.01f)) ? 0 : 1;
		}

		{
			float const Constant = 1.0f / static_cast<float>(std::numeric_limits<glm::i8>::max());

			gli::texture2D Texture(gli::FORMAT_RGBA8_SNORM, gli::texture2D::dim_type(1), 1);
			gli::detail::convert<gli::texture2D, float, glm::i8, gli::defaultp, gli::tvec4, gli::detail::CONVERT_MODE_NORM>::write(Texture, gli::texture2D::dim_type(0), 0, gli::vec4(1.0f, 0.5f, 0.0f, 1.0f));
			gli::vec4 Texel = gli::detail::convert<gli::texture2D, float, glm::i8, gli::defaultp, gli::tvec4, gli::detail::CONVERT_MODE_NORM>::fetch(Texture, gli::texture2D::dim_type(0), 0);

			Error += gli::all(gli::epsilonEqual(Texel, gli::vec4(1.0f, 0.5f, 0.0f, 1.0f), 0.01f)) ? 0 : 1;
		}

		return Error;
	}
}//namespace rgba8_norm

namespace rgba8_srgb
{
	int test()
	{
		int Error = 0;

		{
			gli::texture2D Texture(gli::FORMAT_RGBA8_SRGB, gli::texture2D::dim_type(1), 1);
			gli::detail::convert<gli::texture2D, float, glm::u8, gli::defaultp, gli::tvec4, gli::detail::CONVERT_MODE_SRGB>::write(Texture, gli::texture2D::dim_type(0), 0, gli::vec4(1.0f, 0.5f, 0.0f, 1.0f));
			glm::vec4 Texel = gli::detail::convert<gli::texture2D, float, glm::u8, gli::defaultp, gli::tvec4, gli::detail::CONVERT_MODE_SRGB>::fetch(Texture, gli::texture2D::dim_type(0), 0);

			Error += gli::all(gli::epsilonEqual(Texel, gli::vec4(1.0f, 0.5f, 0.0f, 1.0f), 0.01f)) ? 0 : 1;
		}

		{
			gli::texture2D Texture(gli::FORMAT_RGBA8_SRGB, gli::texture2D::dim_type(1), 1);
			gli::detail::convert<gli::texture2D, double, glm::u8, gli::defaultp, gli::tvec4, gli::detail::CONVERT_MODE_SRGB>::write(Texture, gli::texture2D::dim_type(0), 0, gli::dvec4(1.0f, 0.5f, 0.0f, 1.0f));
			glm::dvec4 Texel = gli::detail::convert<gli::texture2D, double, glm::u8, gli::defaultp, gli::tvec4, gli::detail::CONVERT_MODE_SRGB>::fetch(Texture, gli::texture2D::dim_type(0), 0);

			Error += gli::all(gli::epsilonEqual(Texel, gli::dvec4(1.0f, 0.5f, 0.0f, 1.0f), 0.01)) ? 0 : 1;
		}

		return Error;
	}
}//namespace rgba8_srgb

namespace rgb9e5
{
	int test()
	{
		int Error = 0;

		{
			gli::texture2D Texture(gli::FORMAT_RGB9E5_UFLOAT, gli::texture2D::dim_type(1), 1);
			gli::detail::convert<gli::texture2D, float, glm::u8, gli::defaultp, gli::tvec3, gli::detail::CONVERT_MODE_RGB9E5>::write(Texture, gli::texture2D::dim_type(0), 0, gli::vec3(1.0f, 0.5f, 0.0f));
			glm::vec3 Texel = gli::detail::convert<gli::texture2D, float, glm::u8, gli::defaultp, gli::tvec3, gli::detail::CONVERT_MODE_RGB9E5>::fetch(Texture, gli::texture2D::dim_type(0), 0);

			Error += gli::all(gli::epsilonEqual(Texel, gli::vec3(1.0f, 0.5f, 0.0f), 0.01f)) ? 0 : 1;
		}

		{
			gli::texture2D Texture(gli::FORMAT_RGB9E5_UFLOAT, gli::texture2D::dim_type(1), 1);
			gli::detail::convert<gli::texture2D, double, glm::u8, gli::defaultp, gli::tvec3, gli::detail::CONVERT_MODE_RGB9E5>::write(Texture, gli::texture2D::dim_type(0), 0, gli::dvec3(1.0f, 0.5f, 0.0f));
			glm::dvec3 Texel = gli::detail::convert<gli::texture2D, double, glm::u8, gli::defaultp, gli::tvec3, gli::detail::CONVERT_MODE_RGB9E5>::fetch(Texture, gli::texture2D::dim_type(0), 0);

			Error += gli::all(gli::epsilonEqual(Texel, gli::dvec3(1.0f, 0.5f, 0.0f), 0.01)) ? 0 : 1;
		}

		return Error;
	}
}//namespace rgb9e5

namespace rg11b10f
{
	int test()
	{
		int Error = 0;

		{
			gli::texture2D Texture(gli::FORMAT_RG11B10_UFLOAT, gli::texture2D::dim_type(1), 1);
			gli::detail::convert<gli::texture2D, float, glm::u8, gli::defaultp, gli::tvec3, gli::detail::CONVERT_MODE_RG11B10F>::write(Texture, gli::texture2D::dim_type(0), 0, gli::vec3(1.0f, 0.5f, 0.0f));
			glm::vec3 Texel = gli::detail::convert<gli::texture2D, float, glm::u8, gli::defaultp, gli::tvec3, gli::detail::CONVERT_MODE_RG11B10F>::fetch(Texture, gli::texture2D::dim_type(0), 0);

			Error += gli::all(gli::epsilonEqual(Texel, gli::vec3(1.0f, 0.5f, 0.0f), 0.01f)) ? 0 : 1;
		}

		{
			gli::texture2D Texture(gli::FORMAT_RG11B10_UFLOAT, gli::texture2D::dim_type(1), 1);
			gli::detail::convert<gli::texture2D, double, glm::u8, gli::defaultp, gli::tvec3, gli::detail::CONVERT_MODE_RG11B10F>::write(Texture, gli::texture2D::dim_type(0), 0, gli::dvec3(1.0f, 0.5f, 0.0f));
			glm::dvec3 Texel = gli::detail::convert<gli::texture2D, double, glm::u8, gli::defaultp, gli::tvec3, gli::detail::CONVERT_MODE_RG11B10F>::fetch(Texture, gli::texture2D::dim_type(0), 0);

			Error += gli::all(gli::epsilonEqual(Texel, gli::dvec3(1.0f, 0.5f, 0.0f), 0.01)) ? 0 : 1;
		}

		return Error;
	}
}//namespace rg11b10f

namespace rgb10a2norm
{
	int test()
	{
		int Error = 0;

		{
			gli::texture2D Texture(gli::FORMAT_RGB10A2_UNORM, gli::texture2D::dim_type(1), 1);
			gli::detail::convert<gli::texture2D, float, glm::u8, gli::defaultp, gli::tvec4, gli::detail::CONVERT_MODE_RGB10A2UNORM>::write(Texture, gli::texture2D::dim_type(0), 0, gli::vec4(1.0f, 0.5f, 0.0f, 1.0f));
			glm::vec4 Texel = gli::detail::convert<gli::texture2D, float, glm::u8, gli::defaultp, gli::tvec4, gli::detail::CONVERT_MODE_RGB10A2UNORM>::fetch(Texture, gli::texture2D::dim_type(0), 0);

			Error += gli::all(gli::epsilonEqual(Texel, gli::vec4(1.0f, 0.5f, 0.0f, 1.0f), 0.01f)) ? 0 : 1;
		}

		{
			gli::texture2D Texture(gli::FORMAT_RGB10A2_SNORM, gli::texture2D::dim_type(1), 1);
			gli::detail::convert<gli::texture2D, float, glm::u8, gli::defaultp, gli::tvec4, gli::detail::CONVERT_MODE_RGB10A2SNORM>::write(Texture, gli::texture2D::dim_type(0), 0, gli::vec4(1.0f, 0.5f, 0.0f, 1.0f));
			glm::vec4 Texel = gli::detail::convert<gli::texture2D, float, glm::u8, gli::defaultp, gli::tvec4, gli::detail::CONVERT_MODE_RGB10A2SNORM>::fetch(Texture, gli::texture2D::dim_type(0), 0);

			Error += gli::all(gli::epsilonEqual(Texel, gli::vec4(1.0f, 0.5f, 0.0f, 1.0f), 0.01f)) ? 0 : 1;
		}

		return Error;
	}
}//namespace rgb10a2norm

namespace rgb10a2int
{
	int test()
	{
		int Error = 0;

		{
			gli::uvec4 const Color(255, 127, 0, 3);
			gli::texture2D Texture(gli::FORMAT_RGB10A2_UINT, gli::texture2D::dim_type(1), 1);
			gli::detail::convert<gli::texture2D, gli::uint, glm::u8, gli::defaultp, gli::tvec4, gli::detail::CONVERT_MODE_RGB10A2UINT>::write(Texture, gli::texture2D::dim_type(0), 0, Color);
			glm::uvec4 Texel = gli::detail::convert<gli::texture2D, gli::uint, glm::u8, gli::defaultp, gli::tvec4, gli::detail::CONVERT_MODE_RGB10A2UINT>::fetch(Texture, gli::texture2D::dim_type(0), 0);

			Error += Texel == Color ? 0 : 1;
		}

		{
			gli::ivec4 const Color(127, 63, 0, 1);
			gli::texture2D Texture(gli::FORMAT_RGB10A2_SINT, gli::texture2D::dim_type(1), 1);
			gli::detail::convert<gli::texture2D, gli::sint, glm::u8, gli::defaultp, gli::tvec4, gli::detail::CONVERT_MODE_RGB10A2SINT>::write(Texture, gli::texture2D::dim_type(0), 0, Color);
			glm::ivec4 Texel = gli::detail::convert<gli::texture2D, gli::sint, glm::u8, gli::defaultp, gli::tvec4, gli::detail::CONVERT_MODE_RGB10A2SINT>::fetch(Texture, gli::texture2D::dim_type(0), 0);

			Error += Texel == Color ? 0 : 1;
		}

		return Error;
	}
}//namespace rgb10a2int

int main()
{
	int Error = 0;

	Error += rgba8_int::test();
	Error += rgba8_norm::test();
	Error += rgba8_srgb::test();
	Error += rgb9e5::test();
	Error += rg11b10f::test();
	Error += rgb10a2norm::test();
	Error += rgb10a2int::test();

	return Error;
}

