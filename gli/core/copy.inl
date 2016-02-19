namespace gli{
namespace detail
{
	struct compute_copy_texture_level
	{
		template <typename texture_type>
		static bool call
		(
			texture_type const& TextureSrc, typename texture_type::size_type BaseLevelSrc,
			texture_type& TextureDst, typename texture_type::size_type BaseLevelDst, typename texture_type::size_type MaxLevelDst
		)
		{
			if(TextureSrc.layers() != TextureSrc.layers())
				return false;
			if(TextureSrc.faces() != TextureSrc.faces())
				return false;
			if(TextureSrc.extent(BaseLevelSrc) != TextureDst.extent(BaseLevelDst))
				return false;
			if(MaxLevelDst > TextureSrc.max_level() || MaxLevelDst > TextureDst.max_level())
				return false;

			for(typename texture_type::size_type LayerIndex = 0, LayerCount = TextureSrc.layers(); LayerIndex < LayerCount; ++LayerIndex)
			for(typename texture_type::size_type FaceIndex = 0, FaceCount = TextureSrc.faces(); FaceIndex < FaceCount; ++FaceIndex)
			for(typename texture_type::size_type LevelIndex = BaseLevelDst; LevelIndex <= MaxLevelDst; ++LevelIndex)
			{
				memcpy(
					TextureDst.data(LayerIndex, FaceIndex, BaseLevelDst + LevelIndex),
					TextureSrc.data(LayerIndex, FaceIndex, BaseLevelSrc + LevelIndex),
					TextureDst.size(BaseLevelDst + LevelIndex));
			}

			return true;
		}
	};
}//namespace detail

	template <typename texture_type>
	bool copy
	(
		texture_type const& TextureSrc, typename texture_type::size_type BaseLevelSrc,
		texture_type& TextureDst, typename texture_type::size_type BaseLevelDst
	)
	{
		return detail::compute_copy_texture_level::call(TextureSrc, BaseLevelSrc, TextureDst, BaseLevelDst, BaseLevelDst);
	}

	template <typename texture_type>
	bool copy
	(
		texture_type const& TextureSrc, typename texture_type::size_type BaseLevelSrc,
		texture_type& TextureDst, typename texture_type::size_type BaseLevelDst, typename texture_type::size_type MaxLevelDst
	)
	{
		return detail::compute_copy_texture_level::call(TextureSrc, BaseLevelSrc, TextureDst, BaseLevelDst, MaxLevelDst);
	}
}//namespace gli
