#include <gli/texture2d.hpp>
#include <gli/copy.hpp>
#include <gli/generate_mipmaps.hpp>
#include <gli/comparison.hpp>

namespace gli
{
	bool copy(gli::texture2d const& TextureSrc, int BaseLevelSrc, gli::texture2d& TextureDst, int BaseLevelDst, int MaxLevelDst)
	{
		if(TextureSrc.extent(BaseLevelSrc) != TextureSrc.extent(BaseLevelDst))
			return false;
		if(MaxLevelDst > TextureSrc.max_level() || MaxLevelDst > TextureDst.max_level())
			return false;

		for(int Level = BaseLevelDst; Level <= MaxLevelDst; ++Level)
			memcpy(TextureDst.data(0, 0, BaseLevelDst + Level), TextureSrc.data(0, 0, BaseLevelSrc + Level), TextureDst.size(BaseLevelDst + Level));

		return true;
	}
}//namespace gli

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

int main()
{
	int Error(0);

	Error += texture2d::test();

	return Error;
}
