/// @brief Include to copy textures, images or a subset of either textures or an image. These operations are performed without memory allocations.
/// @file gli/copy.hpp

#pragma once

namespace gli
{
	bool copy(gli::texture2d const& TextureSrc, int BaseLevelSrc, gli::texture2d& TextureDst, int BaseLevelDst, int MaxLevelDst);
}//namespace gli

