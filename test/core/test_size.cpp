#include <gli/texture.hpp>
#include <gli/texture2d.hpp>
#include <gli/comparison.hpp>
#include <gli/view.hpp>
#include <gli/duplicate.hpp>

namespace can_compute_texture_size
{
	int main()
	{
		int Error = 0;

		// Scenario: Compute the size of a specialized 2d texture
		{
			gli::texture2d Texture(gli::FORMAT_RGBA8_UNORM_PACK8, gli::texture2d::extent_type(2), 1);

			Error += Texture.size() == 2 * 2 * sizeof(glm::u8vec4) ? 0 : 1;
			Error += Texture.size(0) == 2 * 2 * sizeof(glm::u8vec4) ? 0 : 1;
			Error += Texture.size<glm::u8vec4>() == 2 * 2 ? 0 : 1;
			Error += Texture.size<glm::u8vec4>(0) == 2 * 2 ? 0 : 1;
		}

		// Scenario: Compute the size of a generic 2d texture
		{
			gli::texture Texture(gli::TARGET_2D, gli::FORMAT_RGBA8_UNORM_PACK8, gli::texture::extent_type(2, 2, 1), 1, 1, 1);

			Error += Texture.size() == 2 * 2 * sizeof(glm::u8vec4) ? 0 : 1;
			Error += Texture.size(0) == 2 * 2 * sizeof(glm::u8vec4) ? 0 : 1;
			Error += Texture.size<glm::u8vec4>() == 2 * 2 ? 0 : 1;
			Error += Texture.size<glm::u8vec4>(0) == 2 * 2 ? 0 : 1;
		}

		// Scenario: Compute the size of a specialized 2d texture with a mipmap chain
		{
			gli::texture2d Texture(gli::FORMAT_RGBA8_UNORM_PACK8, gli::texture2d::extent_type(2));

			Error += Texture.size() == 5 * sizeof(glm::u8vec4) ? 0 : 1;
			Error += Texture.size(0) == 4 * sizeof(glm::u8vec4) ? 0 : 1;
			Error += Texture.size(1) == 1 * sizeof(glm::u8vec4) ? 0 : 1;
			Error += Texture.size<glm::u8vec4>() == 5 ? 0 : 1;
			Error += Texture.size<glm::u8vec4>(0) == 4 ? 0 : 1;
			Error += Texture.size<glm::u8vec4>(1) == 1 ? 0 : 1;
		}

		// Scenario: Compute the size of a generic 2d texture with a mipmap chain
		{
			gli::texture Texture(gli::TARGET_2D, gli::FORMAT_RGBA8_UNORM_PACK8, gli::texture::extent_type(2, 2, 1), 1, 1, 2);

			Error += Texture.size() == 5 * sizeof(glm::u8vec4) ? 0 : 1;
			Error += Texture.size(0) == 4 * sizeof(glm::u8vec4) ? 0 : 1;
			Error += Texture.size(1) == 1 * sizeof(glm::u8vec4) ? 0 : 1;
			Error += Texture.size<glm::u8vec4>() == 5 ? 0 : 1;
			Error += Texture.size<glm::u8vec4>(0) == 4 ? 0 : 1;
			Error += Texture.size<glm::u8vec4>(1) == 1 ? 0 : 1;
		}

		return Error;
	}
}//namespace can_compute_texture_size

namespace can_compute_view_size
{
	int main()
	{
		int Error = 0;

		// Scenario: Compute the size of a specialized 2d texture with a mipmap chain
		{
			gli::texture2d Texture(gli::FORMAT_RGBA8_UNORM_PACK8, gli::texture2d::extent_type(4));
			gli::texture2d View(gli::view(Texture, 1, 2));

			Error += View.size() == 5 * sizeof(glm::u8vec4) ? 0 : 1;
			Error += View.size(0) == 4 * sizeof(glm::u8vec4) ? 0 : 1;
			Error += View.size(1) == 1 * sizeof(glm::u8vec4) ? 0 : 1;
			Error += View.size<glm::u8vec4>() == 5 ? 0 : 1;
			Error += View.size<glm::u8vec4>(0) == 4 ? 0 : 1;
			Error += View.size<glm::u8vec4>(1) == 1 ? 0 : 1;
		}

		// Scenario: Compute the size of a generic 2d texture with a mipmap chain
		{
			gli::texture Texture(gli::TARGET_2D, gli::FORMAT_RGBA8_UNORM_PACK8, gli::texture::extent_type(4, 4, 1), 1, 1, 3);
			gli::texture View(gli::view(gli::texture2d(Texture), 1, 2));

			Error += View.size() == 5 * sizeof(glm::u8vec4) ? 0 : 1;
			Error += View.size(0) == 4 * sizeof(glm::u8vec4) ? 0 : 1;
			Error += View.size(1) == 1 * sizeof(glm::u8vec4) ? 0 : 1;
			Error += View.size<glm::u8vec4>() == 5 ? 0 : 1;
			Error += View.size<glm::u8vec4>(0) == 4 ? 0 : 1;
			Error += View.size<glm::u8vec4>(1) == 1 ? 0 : 1;
		}

		// Scenario: Compute the size of a specialized 2d array texture with a mipmap chain
		{
			gli::texture2d_array Texture(gli::FORMAT_RGBA8_UNORM_PACK8, gli::texture2d::extent_type(4), 2);
			gli::texture2d_array View(gli::view(Texture, 1, 1, 1, 2));

			Error += View.size() == 5 * sizeof(glm::u8vec4) ? 0 : 1;
			Error += View.size(0) == 4 * sizeof(glm::u8vec4) ? 0 : 1;
			Error += View.size(1) == 1 * sizeof(glm::u8vec4) ? 0 : 1;
			Error += View.size<glm::u8vec4>() == 5 ? 0 : 1;
			Error += View.size<glm::u8vec4>(0) == 4 ? 0 : 1;
			Error += View.size<glm::u8vec4>(1) == 1 ? 0 : 1;
		}

		return Error;
	}
}//namespace can_compute_view_size

int main()
{
	int Error = 0;

	Error += can_compute_texture_size::main();
	Error += can_compute_view_size::main();

	return Error;
}


