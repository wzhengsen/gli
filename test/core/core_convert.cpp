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
/// @date 2016-10-05 / 2015-10-05
/// @author Christophe Riccio
///////////////////////////////////////////////////////////////////////////////////

#include <gli/load.hpp>
#include <gli/save.hpp>
#include <gli/convert.hpp>
#include <gli/comparison.hpp>
#include <glm/gtc/epsilon.hpp>
#include <ctime>

namespace
{
	std::string path(const char* filename)
	{
		return std::string(SOURCE_DIR) + "/data/" + filename;
	}

	struct params
	{
		params(std::string const & Filename, gli::format Format)
			: Filename(Filename)
			, Format(Format)
		{}

		std::string Filename;
		gli::format Format;
	};
}//namespace

namespace rgb10a2norm
{
	int test()
	{
		int Error = 0;

		{
			gli::texture1D TextureSrc(gli::FORMAT_RGB10A2_UNORM_PACK32, gli::texture1D::texelcoord_type(4));
			TextureSrc.clear(glm::u8vec4(255, 127, 0, 255));

			gli::texture1D TextureDst = gli::convert(TextureSrc, gli::FORMAT_RGB10A2_UNORM_PACK32);

			Error += TextureSrc == TextureDst ? 0 : 1;
		}

		{
			gli::texture1DArray TextureSrc(gli::FORMAT_RGB10A2_UNORM_PACK32, gli::texture1DArray::texelcoord_type(4), 2);
			TextureSrc.clear(glm::u8vec4(255, 127, 0, 255));

			gli::texture1DArray TextureDst = gli::convert(TextureSrc, gli::FORMAT_RGB10A2_UNORM_PACK32);

			Error += TextureSrc == TextureDst ? 0 : 1;
		}

		{
			gli::texture2D TextureSrc(gli::FORMAT_RGB10A2_UNORM_PACK32, gli::texture2D::texelcoord_type(4));
			TextureSrc.clear(glm::u8vec4(255, 127, 0, 255));

			gli::texture2D TextureDst = gli::convert(TextureSrc, gli::FORMAT_RGB10A2_UNORM_PACK32);

			Error += TextureSrc == TextureDst ? 0 : 1;
		}

		{
			gli::texture2DArray TextureSrc(gli::FORMAT_RGB10A2_UNORM_PACK32, gli::texture2DArray::texelcoord_type(4), 2);
			TextureSrc.clear(glm::u8vec4(255, 127, 0, 255));

			gli::texture2DArray TextureDst = gli::convert(TextureSrc, gli::FORMAT_RGB10A2_UNORM_PACK32);

			Error += TextureSrc == TextureDst ? 0 : 1;
		}

		{
			gli::texture3D TextureSrc(gli::FORMAT_RGB10A2_UNORM_PACK32, gli::texture3D::texelcoord_type(4));
			TextureSrc.clear(glm::u8vec4(255, 127, 0, 255));

			gli::texture3D TextureDst = gli::convert(TextureSrc, gli::FORMAT_RGB10A2_UNORM_PACK32);

			Error += TextureSrc == TextureDst ? 0 : 1;
		}

		{
			gli::textureCube TextureSrc(gli::FORMAT_RGB10A2_UNORM_PACK32, gli::textureCube::texelcoord_type(4), 2);
			TextureSrc.clear(glm::u8vec4(255, 127, 0, 255));

			gli::textureCube TextureDst = gli::convert(TextureSrc, gli::FORMAT_RGB10A2_UNORM_PACK32);

			Error += TextureSrc == TextureDst ? 0 : 1;
		}

		{
			gli::textureCubeArray TextureSrc(gli::FORMAT_RGB10A2_UNORM_PACK32, gli::textureCubeArray::texelcoord_type(4), 2);
			TextureSrc.clear(glm::u8vec4(255, 127, 0, 255));

			gli::textureCubeArray TextureDst = gli::convert(TextureSrc, gli::FORMAT_RGB10A2_UNORM_PACK32);

			Error += TextureSrc == TextureDst ? 0 : 1;
		}

		return Error;
	}
}//namespace rgb10a2norm

namespace load_file
{
	int test()
	{
		int Error(0);

		gli::texture2D TextureA(gli::load(path("kueken7_rgba16_sfloat.ktx")));
		GLI_ASSERT(!TextureA.empty());

		gli::texture2D Convert = gli::convert(TextureA, gli::FORMAT_RG11B10_UFLOAT_PACK32);

		gli::save(Convert, "kueken7_rg11b10_ufloat.dds");
		gli::save(Convert, "kueken7_rg11b10_ufloat.ktx");

		gli::texture2D TextureDDS(gli::load("kueken7_rg11b10_ufloat.dds"));
		GLI_ASSERT(!TextureDDS.empty());
		gli::texture2D TextureKTX(gli::load("kueken7_rg11b10_ufloat.ktx"));
		GLI_ASSERT(!TextureKTX.empty());

		Error += TextureDDS == TextureKTX ? 0 : 1;
		Error += TextureDDS == Convert ? 0 : 1;

		GLI_ASSERT(!Error);

		return Error;
	}
}//namespace load_file

int main()
{
	int Error = 0;

	Error += rgb10a2norm::test();
	Error += load_file::test();

	return Error;
}

