#include <gli/copy.hpp>
#include <gli/generate_mipmaps.hpp>
#include <gli/comparison.hpp>

namespace texture1d
{
	int copy_levels()
	{
		int Error = 0;

		{
			gli::texture1d TextureSrc(gli::FORMAT_RGBA8_UNORM_PACK8, gli::texture1d::extent_type(2));
			TextureSrc.clear(glm::u8vec4(193, 127, 0, 255));

			gli::texture1d TextureDst(gli::FORMAT_RGBA8_UNORM_PACK8, gli::texture1d::extent_type(2));
			TextureDst.clear(glm::u8vec4(4, 3, 2, 1));

			gli::copy(TextureSrc, 0, TextureDst, 0, 1);
			gli::generate_mipmaps(TextureDst, gli::FILTER_LINEAR);

			Error += TextureDst == TextureSrc ? 0 : 1;
		}
		{
			gli::texture TextureSrc(gli::TARGET_1D, gli::FORMAT_RGBA8_UNORM_PACK8, gli::texture::extent_type(2, 1, 1), 1, 2, 1);
			TextureSrc.clear(glm::u8vec4(193, 127, 0, 255));
			
			gli::texture TextureDst(gli::TARGET_1D, gli::FORMAT_RGBA8_UNORM_PACK8, gli::texture::extent_type(2, 1, 1), 1, 2, 1);
			TextureDst.clear(glm::u8vec4(4, 3, 2, 1));
			
			gli::copy(TextureSrc, 0, TextureDst, 0, 1);
			
			Error += TextureDst == TextureSrc ? 0 : 1;
		}
		
		return Error;
	}

	int main()
	{
		int Error = 0;

		Error += copy_levels();

		return Error;
	}
}//namespace texture1d

namespace texture1d_array
{
	int copy_levels()
	{
		int Error = 0;

		{
			gli::texture1d_array TextureSrc(gli::FORMAT_RGBA8_UNORM_PACK8, gli::texture1d_array::extent_type(2), 2);
			TextureSrc.clear(glm::u8vec4(193, 127, 0, 255));

			gli::texture1d_array TextureDst(gli::FORMAT_RGBA8_UNORM_PACK8, gli::texture1d_array::extent_type(2), 2);
			TextureDst.clear(glm::u8vec4(4, 3, 2, 1));

			gli::copy(TextureSrc, 0, TextureDst, 0, 1);
			gli::generate_mipmaps(TextureDst, gli::FILTER_LINEAR);

			Error += TextureDst == TextureSrc ? 0 : 1;
		}
		{
			gli::texture TextureSrc(gli::TARGET_1D_ARRAY, gli::FORMAT_RGBA8_UNORM_PACK8, gli::texture::extent_type(2, 1, 1), 2, 2, 1);
			TextureSrc.clear(glm::u8vec4(193, 127, 0, 255));
			
			gli::texture TextureDst(gli::TARGET_1D_ARRAY, gli::FORMAT_RGBA8_UNORM_PACK8, gli::texture::extent_type(2, 1, 1), 2, 2, 1);
			TextureDst.clear(glm::u8vec4(4, 3, 2, 1));
			
			gli::copy(TextureSrc, 0, TextureDst, 0, 1);
			
			Error += TextureDst == TextureSrc ? 0 : 1;
		}
		
		return Error;
	}

	int main()
	{
		int Error = 0;

		Error += copy_levels();

		return Error;
	}
}//namespace texture1d_array

namespace texture2d
{
	int copy_level()
	{
		int Error = 0;

		gli::texture2d TextureSrc(gli::FORMAT_RGBA8_UNORM_PACK8, gli::texture2d::extent_type(2), 1);
		TextureSrc.clear(glm::u8vec4(193, 127, 0, 255));

		gli::texture2d TextureDst(gli::FORMAT_RGBA8_UNORM_PACK8, gli::texture2d::extent_type(2), 1);
		TextureDst.clear(glm::u8vec4(4, 3, 2, 1));

		gli::copy(TextureSrc, 0, TextureDst, 0);
		gli::generate_mipmaps(TextureDst, gli::FILTER_LINEAR);

		Error += TextureDst == TextureSrc ? 0 : 1;

		return Error;
	}

	int copy_levels()
	{
		int Error = 0;

		{
			gli::texture2d TextureSrc(gli::FORMAT_RGBA8_UNORM_PACK8, gli::texture2d::extent_type(2));
			TextureSrc.clear(glm::u8vec4(193, 127, 0, 255));

			gli::texture2d TextureDst(gli::FORMAT_RGBA8_UNORM_PACK8, gli::texture2d::extent_type(2));
			TextureDst.clear(glm::u8vec4(4, 3, 2, 1));

			gli::copy(TextureSrc, 0, TextureDst, 0, 1);
			gli::generate_mipmaps(TextureDst, gli::FILTER_LINEAR);

			Error += TextureDst == TextureSrc ? 0 : 1;
		}
		{
			gli::texture TextureSrc(gli::TARGET_2D, gli::FORMAT_RGBA8_UNORM_PACK8, gli::texture::extent_type(2, 2, 1), 1, 2, 1);
			TextureSrc.clear(glm::u8vec4(193, 127, 0, 255));

			gli::texture TextureDst(gli::TARGET_2D, gli::FORMAT_RGBA8_UNORM_PACK8, gli::texture::extent_type(2, 2, 1), 1, 2, 1);
			TextureDst.clear(glm::u8vec4(4, 3, 2, 1));

			gli::copy(TextureSrc, 0, TextureDst, 0, 1);

			Error += TextureDst == TextureSrc ? 0 : 1;
		}

		return Error;
	}

	int main()
	{
		int Error = 0;

		Error += copy_level();
		Error += copy_levels();

		return Error;
	}
}//namespace texture2d

namespace texture2d_array
{
	int copy_levels()
	{
		int Error = 0;

		{
			gli::texture2d_array TextureSrc(gli::FORMAT_RGBA8_UNORM_PACK8, gli::texture2d_array::extent_type(2), 2);
			TextureSrc.clear(glm::u8vec4(193, 127, 0, 255));

			gli::texture2d_array TextureDst(gli::FORMAT_RGBA8_UNORM_PACK8, gli::texture2d_array::extent_type(2), 2);
			TextureDst.clear(glm::u8vec4(4, 3, 2, 1));

			gli::copy(TextureSrc, 0, TextureDst, 0, 1);
			gli::generate_mipmaps(TextureDst, gli::FILTER_LINEAR);

			Error += TextureDst == TextureSrc ? 0 : 1;
		}
		{
			gli::texture TextureSrc(gli::TARGET_2D_ARRAY, gli::FORMAT_RGBA8_UNORM_PACK8, gli::texture::extent_type(2, 2, 1), 2, 2, 1);
			TextureSrc.clear(glm::u8vec4(193, 127, 0, 255));

			gli::texture TextureDst(gli::TARGET_2D_ARRAY, gli::FORMAT_RGBA8_UNORM_PACK8, gli::texture::extent_type(2, 2, 1), 2, 2, 1);
			TextureDst.clear(glm::u8vec4(4, 3, 2, 1));

			gli::copy(TextureSrc, 0, TextureDst, 0, 1);

			Error += TextureDst == TextureSrc ? 0 : 1;
		}

		return Error;
	}

	int main()
	{
		int Error = 0;

		Error += copy_levels();

		return Error;
	}
}//namespace texture2d_array

namespace texture3d
{
	int copy_levels()
	{
		int Error = 0;

		gli::texture3d TextureSrc(gli::FORMAT_RGBA8_UNORM_PACK8, gli::texture3d::extent_type(2));
		TextureSrc.clear(glm::u8vec4(193, 127, 0, 255));

		gli::texture3d TextureDst(gli::FORMAT_RGBA8_UNORM_PACK8, gli::texture3d::extent_type(2));
		TextureDst.clear(glm::u8vec4(4, 3, 2, 1));

		gli::copy(TextureSrc, 0, TextureDst, 0, 1);
		gli::generate_mipmaps(TextureDst, gli::FILTER_LINEAR);

		Error += TextureDst == TextureSrc ? 0 : 1;

		return Error;
	}

	int main()
	{
		int Error = 0;

		Error += copy_levels();

		return Error;
	}
}//namespace texture3d


namespace texture_cube
{
	int copy_levels()
	{
		int Error = 0;

		gli::texture_cube TextureSrc(gli::FORMAT_RGBA8_UNORM_PACK8, gli::texture_cube::extent_type(2));
		TextureSrc.clear(glm::u8vec4(193, 127, 0, 255));

		gli::texture_cube TextureDst(gli::FORMAT_RGBA8_UNORM_PACK8, gli::texture_cube::extent_type(2));
		TextureDst.clear(glm::u8vec4(4, 3, 2, 1));

		gli::copy(TextureSrc, 0, TextureDst, 0, 1);
		gli::generate_mipmaps(TextureDst, gli::FILTER_LINEAR);

		Error += TextureDst == TextureSrc ? 0 : 1;

		return Error;
	}

	int main()
	{
		int Error = 0;

		Error += copy_levels();

		return Error;
	}
}//namespace texture_cube

namespace texture_cube_array
{
	int copy_levels()
	{
		int Error = 0;

		gli::texture_cube_array TextureSrc(gli::FORMAT_RGBA8_UNORM_PACK8, gli::texture_cube_array::extent_type(2), 2);
		TextureSrc.clear(glm::u8vec4(193, 127, 0, 255));

		gli::texture_cube_array TextureDst(gli::FORMAT_RGBA8_UNORM_PACK8, gli::texture_cube_array::extent_type(2), 2);
		TextureDst.clear(glm::u8vec4(4, 3, 2, 1));

		gli::copy(TextureSrc, 0, TextureDst, 0, 1);
		gli::generate_mipmaps(TextureDst, gli::FILTER_LINEAR);

		Error += TextureDst == TextureSrc ? 0 : 1;

		return Error;
	}

	int main()
	{
		int Error = 0;

		Error += copy_levels();

		return Error;
	}
}//namespace texture_cube_array

int main()
{
	int Error(0);

	Error += texture1d::main();
	Error += texture1d_array::main();
	Error += texture2d::main();
	Error += texture2d_array::main();
	Error += texture3d::main();
	Error += texture_cube::main();
	Error += texture_cube_array::main();

	return Error;
}
