#include <gli/copy.hpp>
#include <gli/generate_mipmaps.hpp>
#include <gli/comparison.hpp>

namespace helper
{
	gli::texture2d make2d(gli::format Format, gli::texture2d::extent_type Extent, glm::u8vec4 const& ClearColor)
	{
		gli::texture2d Texture(Format, Extent);
		Texture.clear(ClearColor);
		return Texture;
	}

	gli::texture2d_array make2d_array(gli::format Format, gli::texture2d_array::extent_type Extent, gli::texture2d_array::size_type Layers, glm::u8vec4 const& ClearColor)
	{
		gli::texture2d_array Texture(Format, Extent, Layers);
		Texture.clear(ClearColor);
		return Texture;
	}

	gli::texture_cube_array make_cube_array(gli::format Format, gli::texture2d_array::extent_type Extent, gli::texture2d_array::size_type Layers, glm::u8vec4 const& ClearColor)
	{
		gli::texture_cube_array Texture(Format, Extent, Layers);
		Texture.clear(ClearColor);
		return Texture;
	}
}//namespace helper

// Can copy all levels from a source 2d texture to a destination texture of equivalent storage shape
static int can_copy_level_all_levels()
{
	int Error = 0;

	gli::texture2d const TextureExpected = helper::make2d(gli::FORMAT_RGBA8_UNORM_PACK8, gli::texture2d::extent_type(8), glm::u8vec4(193, 127, 0, 255));

	// Scenario: Using gli::copy on a specialized texture 2d to copy all images at once
	{
		gli::texture2d TextureScenario(TextureExpected.format(), TextureExpected.extent());
		Error += TextureExpected != TextureScenario ? 0 : 1;

		gli::copy_level(TextureExpected, 0, TextureScenario, 0, TextureScenario.levels());
		Error += TextureExpected == TextureScenario ? 0 : 1;
	}

	// Scenario: Using gli::copy on a specialized texture 2d to copy all images one by one
	{
		gli::texture2d TextureScenario(TextureExpected.format(), TextureExpected.extent());
		Error += TextureExpected != TextureScenario ? 0 : 1;

		for(gli::size_t LevelIndex = 0, LevelCount = TextureExpected.levels(); LevelIndex < LevelCount; ++LevelIndex)
			gli::copy_level(TextureExpected, LevelIndex, TextureScenario, LevelIndex);
		Error += TextureExpected == TextureScenario ? 0 : 1;
	}

	// Scenario: Using gli::copy on a specialized texture 2d array
	{
		gli::texture2d_array TextureScenario(TextureExpected.format(), TextureExpected.extent(), 1);
		Error += TextureExpected != gli::texture2d(TextureScenario) ? 0 : 1;

		gli::copy_level(TextureExpected, 0, TextureScenario, 0, TextureScenario.levels());
		Error += TextureExpected == gli::texture2d(TextureScenario) ? 0 : 1;
	}

	// Scenario: Using gli::copy on a generic texture 2d to copy all images at once
	{
		gli::texture::extent_type const TextureExtent(TextureExpected.extent(), 1);
		gli::texture TextureScenario(TextureExpected.target(), TextureExpected.format(), TextureExtent, 1, 1, gli::levels(TextureExtent));
		Error += TextureExpected != TextureScenario ? 0 : 1;

		gli::copy_level(TextureExpected, 0, TextureScenario, 0, TextureExpected.levels());
		Error += TextureExpected == TextureScenario ? 0 : 1;
	}

	// Scenario: Using gli::copy on a generic texture 2d to copy all images one by one
	{
		gli::texture::extent_type const TextureExtent(TextureExpected.extent(), 1);
		gli::texture TextureScenario(TextureExpected.target(), TextureExpected.format(), TextureExtent, 1, 1, gli::levels(TextureExtent));
		Error += TextureExpected != TextureScenario ? 0 : 1;

		for(gli::size_t LevelIndex = 0, LevelCount = TextureExpected.levels(); LevelIndex < LevelCount; ++LevelIndex)
			gli::copy_level(TextureExpected, LevelIndex, TextureScenario, LevelIndex);
		Error += TextureExpected == TextureScenario ? 0 : 1;
	}

	// Scenario: Using member gli::texture::copy on a specialized texture 2d
	{
		gli::texture2d TextureScenario(TextureExpected.format(), TextureExpected.extent());
		Error += TextureExpected != TextureScenario ? 0 : 1;

		for(gli::size_t Level = 0; Level < TextureScenario.levels(); ++Level)
			TextureScenario.copy(TextureExpected, 0, 0, Level, 0, 0, Level);
		Error += TextureExpected == TextureScenario ? 0 : 1;
	}

	// Scenario: Using member gli::texture::copy on a specialized texture 2d array
	{
		gli::texture2d_array TextureScenario(TextureExpected.format(), TextureExpected.extent(), 1);
		Error += TextureExpected != gli::texture2d(TextureScenario) ? 0 : 1;

		for(gli::size_t Level = 0; Level < TextureScenario.levels(); ++Level)
			TextureScenario.copy(TextureExpected, 0, 0, Level, 0, 0, Level);
		Error += TextureExpected == gli::texture2d(TextureScenario) ? 0 : 1;
	}

	// Scenario: Using member gli::texture::copy on a generic texture 2d
	{
		gli::texture::extent_type const TextureExtent(TextureExpected.extent(), 1);
		gli::texture TextureScenario(TextureExpected.target(), TextureExpected.format(), TextureExtent, 1, 1, gli::levels(TextureExtent));
		Error += TextureExpected != TextureScenario ? 0 : 1;

		for(gli::size_t Level = 0; Level < TextureExpected.levels(); ++Level)
			TextureScenario.copy(TextureExpected, 0, 0, Level, 0, 0, Level);
		Error += TextureExpected == TextureScenario ? 0 : 1;
	}

	return Error;
}

// Can copy a single level from a source texture to a destination texture of equivalent storage shape
static int can_copy_level_single_level()
{
	int Error = 0;

	glm::u8vec4 const DarkOrange(193, 127, 0, 255);
	glm::u8vec4 const LightOrange(255, 191, 127, 255);

	gli::texture_cube_array TextureExpected(gli::FORMAT_RGBA8_UNORM_PACK8, gli::texture_cube_array::extent_type(2), 1);
	TextureExpected.clear(DarkOrange);
	for(gli::size_t LayerIndex = 0, LayerCount = TextureExpected.layers(); LayerIndex < LayerCount; ++LayerIndex)
	for(gli::size_t FaceIndex = 0, FaceCount = TextureExpected.faces(); FaceIndex < FaceCount; ++FaceIndex)
		TextureExpected.clear(LayerIndex, FaceIndex, 1, LightOrange);

	// Scenario: Using gli::copy_level on a specialized texture cube array to copy all images one by one
	{
		gli::texture_cube_array TextureScenario(TextureExpected.format(), TextureExpected.extent(), TextureExpected.layers());
		TextureScenario.clear(DarkOrange);
		Error += TextureExpected != TextureScenario ? 0 : 1;

		gli::copy_level(TextureExpected, 1, TextureScenario, 1);
		Error += TextureExpected == TextureScenario ? 0 : 1;
	}

	// Scenario: copying levels from storage of different shape.
	{
		gli::texture_cube_array TextureSource(gli::FORMAT_RGBA8_UNORM_PACK8, TextureExpected.extent() >> 1, TextureExpected.layers(), 1);
		TextureSource.clear(LightOrange);

		gli::texture_cube_array TextureScenario(TextureExpected.format(), TextureExpected.extent(), TextureExpected.layers());
		TextureScenario.clear(DarkOrange);
		Error += TextureExpected != TextureScenario ? 0 : 1;

		gli::copy_level(TextureSource, 0, TextureScenario, 1);
		Error += TextureExpected == TextureScenario ? 0 : 1;
	}

	return Error;
}


// Can copy all levels from various source texture target to a destination texture target of equivalent storage shape
static int can_copy_level_texture_targets()
{
	int Error = 0;

	// Texture 2d array
	{
		gli::texture2d_array const Texture2DArrayExpected = helper::make2d_array(gli::FORMAT_RGBA8_UNORM_PACK8, gli::texture2d::extent_type(8), 4, glm::u8vec4(193, 127, 0, 255));

		// Scenario: Using gli::copy on a specialized texture 2d array to copy all images at once
		{
			gli::texture2d_array TextureScenario(Texture2DArrayExpected.format(), Texture2DArrayExpected.extent(), Texture2DArrayExpected.layers());
			Error += Texture2DArrayExpected != TextureScenario ? 0 : 1;

			gli::copy_level(Texture2DArrayExpected, 0, TextureScenario, 0, TextureScenario.levels());
			Error += Texture2DArrayExpected == TextureScenario ? 0 : 1;
		}

		// Scenario: Using gli::copy on a specialized texture 2d array to copy all images one by one
		{
			gli::texture2d_array TextureScenario(Texture2DArrayExpected.format(), Texture2DArrayExpected.extent(), Texture2DArrayExpected.layers());
			Error += Texture2DArrayExpected != TextureScenario ? 0 : 1;

			for(gli::size_t LevelIndex = 0, LevelCount = Texture2DArrayExpected.levels(); LevelIndex < LevelCount; ++LevelIndex)
				gli::copy_level(Texture2DArrayExpected, LevelIndex, TextureScenario, LevelIndex);
			Error += Texture2DArrayExpected == TextureScenario ? 0 : 1;
		}
	}

	// Texture cube array
	{
		gli::texture_cube_array const TextureCubeMapExpected = helper::make_cube_array(gli::FORMAT_RGBA8_UNORM_PACK8, gli::texture_cube_array::extent_type(8), 4, glm::u8vec4(193, 127, 0, 255));

		// Scenario: Using gli::copy on a specialized texture cube array to copy all images at once
		{
			gli::texture_cube_array TextureScenario(TextureCubeMapExpected.format(), TextureCubeMapExpected.extent(), TextureCubeMapExpected.layers());
			Error += TextureCubeMapExpected != TextureScenario ? 0 : 1;

			gli::copy_level(TextureCubeMapExpected, 0, TextureScenario, 0, TextureScenario.levels());
			Error += TextureCubeMapExpected == TextureScenario ? 0 : 1;
		}

		// Scenario: Using gli::copy on a specialized texture cube array to copy all images one by one
		{
			gli::texture_cube_array TextureScenario(TextureCubeMapExpected.format(), TextureCubeMapExpected.extent(), TextureCubeMapExpected.layers());
			Error += TextureCubeMapExpected != TextureScenario ? 0 : 1;

			for(gli::size_t LevelIndex = 0, LevelCount = TextureCubeMapExpected.levels(); LevelIndex < LevelCount; ++LevelIndex)
				gli::copy_level(TextureCubeMapExpected, LevelIndex, TextureScenario, LevelIndex);
			Error += TextureCubeMapExpected == TextureScenario ? 0 : 1;
		}
	}

	return Error;
}

int main()
{
	int Error = 0;

	Error += can_copy_level_all_levels();
	Error += can_copy_level_single_level();
	Error += can_copy_level_texture_targets();

	return Error;
}
