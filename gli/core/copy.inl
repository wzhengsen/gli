namespace gli{
namespace detail
{
	template <typename texture_type>
	bool copy_texture(texture_type const& TextureSrc, int BaseLevelSrc, texture_type& TextureDst, int BaseLevelDst, int MaxLevelDst)
	{
		if(TextureSrc.extent(BaseLevelSrc) != TextureSrc.extent(BaseLevelDst))
			return false;
		if(MaxLevelDst > TextureSrc.max_level() || MaxLevelDst > TextureDst.max_level())
			return false;

		for(int Level = BaseLevelDst; Level <= MaxLevelDst; ++Level)
			memcpy(TextureDst.data(0, 0, BaseLevelDst + Level), TextureSrc.data(0, 0, BaseLevelSrc + Level), TextureDst.size(BaseLevelDst + Level));

		return true;
	}
}//namespace detail

	bool copy(gli::texture1d const& TextureSrc, int BaseLevelSrc, gli::texture1d& TextureDst, int BaseLevelDst, int MaxLevelDst)
	{
		return detail::copy_texture(TextureSrc, BaseLevelSrc, TextureDst, BaseLevelDst, MaxLevelDst);
	}

	bool copy(gli::texture2d const& TextureSrc, int BaseLevelSrc, gli::texture2d& TextureDst, int BaseLevelDst, int MaxLevelDst)
	{
		return detail::copy_texture(TextureSrc, BaseLevelSrc, TextureDst, BaseLevelDst, MaxLevelDst);
	}

	bool copy(gli::texture3d const& TextureSrc, int BaseLevelSrc, gli::texture3d& TextureDst, int BaseLevelDst, int MaxLevelDst)
	{
		return detail::copy_texture(TextureSrc, BaseLevelSrc, TextureDst, BaseLevelDst, MaxLevelDst);
	}
}//namespace gli
