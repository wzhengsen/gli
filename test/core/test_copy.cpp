#include <gli/copy.hpp>
#include <gli/generate_mipmaps.hpp>
#include <gli/comparison.hpp>

namespace texture1d
{
	int test()
	{
		int Error = 0;

		gli::texture1d TextureSrc(gli::FORMAT_RGBA8_UNORM_PACK8, gli::texture1d::extent_type(2));
		TextureSrc.clear(glm::u8vec4(193, 127, 0, 255));

		gli::texture1d TextureDst(gli::FORMAT_RGBA8_UNORM_PACK8, gli::texture1d::extent_type(2));
		TextureDst.clear(glm::u8vec4(4, 3, 2, 1));

		gli::copy(TextureSrc, 0, TextureDst, 0, 0);
		gli::generate_mipmaps(TextureDst, gli::FILTER_LINEAR);

		Error += TextureDst == TextureSrc ? 0 : 1;

		return Error;
	}
}//namespace texture1d

namespace texture1d_array
{
	int test()
	{
		int Error = 0;

		gli::texture1d_array TextureSrc(gli::FORMAT_RGBA8_UNORM_PACK8, gli::texture1d_array::extent_type(2), 2);
		TextureSrc.clear(glm::u8vec4(193, 127, 0, 255));

		gli::texture1d_array TextureDst(gli::FORMAT_RGBA8_UNORM_PACK8, gli::texture1d_array::extent_type(2), 2);
		TextureDst.clear(glm::u8vec4(4, 3, 2, 1));

		gli::copy(TextureSrc, 0, TextureDst, 0, 0);
		gli::generate_mipmaps(TextureDst, gli::FILTER_LINEAR);

		Error += TextureDst == TextureSrc ? 0 : 1;

		return Error;
	}
}//namespace texture1d_array

namespace texture2d
{
	int test()
	{
		int Error = 0;

		gli::texture2d TextureSrc(gli::FORMAT_RGBA8_UNORM_PACK8, gli::texture2d::extent_type(2));
		TextureSrc.clear(glm::u8vec4(193, 127, 0, 255));

		gli::texture2d TextureDst(gli::FORMAT_RGBA8_UNORM_PACK8, gli::texture2d::extent_type(2));
		TextureDst.clear(glm::u8vec4(4, 3, 2, 1));

		gli::copy(TextureSrc, 0, TextureDst, 0, 0);
		gli::generate_mipmaps(TextureDst, gli::FILTER_LINEAR);

		Error += TextureDst == TextureSrc ? 0 : 1;

		return Error;
	}
}//namespace texture2d

namespace texture2d_array
{
	int test()
	{
		int Error = 0;

		gli::texture2d_array TextureSrc(gli::FORMAT_RGBA8_UNORM_PACK8, gli::texture2d_array::extent_type(2), 2);
		TextureSrc.clear(glm::u8vec4(193, 127, 0, 255));

		gli::texture2d_array TextureDst(gli::FORMAT_RGBA8_UNORM_PACK8, gli::texture2d_array::extent_type(2), 2);
		TextureDst.clear(glm::u8vec4(4, 3, 2, 1));

		gli::copy(TextureSrc, 0, TextureDst, 0, 0);
		gli::generate_mipmaps(TextureDst, gli::FILTER_LINEAR);

		Error += TextureDst == TextureSrc ? 0 : 1;

		return Error;
	}
}//namespace texture2d_array

namespace texture3d
{
	int test()
	{
		int Error = 0;

		gli::texture3d TextureSrc(gli::FORMAT_RGBA8_UNORM_PACK8, gli::texture3d::extent_type(2));
		TextureSrc.clear(glm::u8vec4(193, 127, 0, 255));

		gli::texture3d TextureDst(gli::FORMAT_RGBA8_UNORM_PACK8, gli::texture3d::extent_type(2));
		TextureDst.clear(glm::u8vec4(4, 3, 2, 1));

		gli::copy(TextureSrc, 0, TextureDst, 0, 0);
		gli::generate_mipmaps(TextureDst, gli::FILTER_LINEAR);

		Error += TextureDst == TextureSrc ? 0 : 1;

		return Error;
	}
}//namespace texture3d


namespace texture_cube
{
	int test()
	{
		int Error = 0;

		gli::texture_cube TextureSrc(gli::FORMAT_RGBA8_UNORM_PACK8, gli::texture_cube::extent_type(2));
		TextureSrc.clear(glm::u8vec4(193, 127, 0, 255));

		gli::texture_cube TextureDst(gli::FORMAT_RGBA8_UNORM_PACK8, gli::texture_cube::extent_type(2));
		TextureDst.clear(glm::u8vec4(4, 3, 2, 1));

		gli::copy(TextureSrc, 0, TextureDst, 0, 0);
		gli::generate_mipmaps(TextureDst, gli::FILTER_LINEAR);

		Error += TextureDst == TextureSrc ? 0 : 1;

		return Error;
	}
}//namespace texture_cube

namespace texture_cube_array
{
	int test()
	{
		int Error = 0;

		gli::texture_cube_array TextureSrc(gli::FORMAT_RGBA8_UNORM_PACK8, gli::texture_cube_array::extent_type(2), 2);
		TextureSrc.clear(glm::u8vec4(193, 127, 0, 255));

		gli::texture_cube_array TextureDst(gli::FORMAT_RGBA8_UNORM_PACK8, gli::texture_cube_array::extent_type(2), 2);
		TextureDst.clear(glm::u8vec4(4, 3, 2, 1));

		gli::copy(TextureSrc, 0, TextureDst, 0, 0);
		gli::generate_mipmaps(TextureDst, gli::FILTER_LINEAR);

		Error += TextureDst == TextureSrc ? 0 : 1;

		return Error;
	}
}//namespace texture_cube_array

int main()
{
	int Error(0);

	Error += texture1d::test();
	Error += texture1d_array::test();
	Error += texture2d::test();
	Error += texture2d_array::test();
	Error += texture3d::test();
	Error += texture_cube::test();
	Error += texture_cube_array::test();

	return Error;
}
