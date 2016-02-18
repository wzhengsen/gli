/// @brief Include to copy textures, images or a subset of either textures or an image. These operations are performed without memory allocations.
/// @file gli/copy.hpp

#pragma once

#include "image.hpp"
#include "texture1d.hpp"
#include "texture1d_array.hpp"
#include "texture2d.hpp"
#include "texture2d_array.hpp"
#include "texture3d.hpp"
#include "texture_cube.hpp"
#include "texture_cube_array.hpp"

namespace gli
{
	bool copy(gli::texture1d const& TextureSrc, int BaseLevelSrc, gli::texture1d& TextureDst, int BaseLevelDst, int MaxLevelDst);

	bool copy(gli::texture2d const& TextureSrc, int BaseLevelSrc, gli::texture2d& TextureDst, int BaseLevelDst, int MaxLevelDst);

	bool copy(gli::texture3d const& TextureSrc, int BaseLevelSrc, gli::texture3d& TextureDst, int BaseLevelDst, int MaxLevelDst);
}//namespace gli

#include "./core/copy.inl"
