namespace gli{
namespace detail
{
	inline void duplicate_images
	(
		texture const & Src, texture & Dst,
		texture::size_type BaseLayer, texture::size_type MaxLayer,
		texture::size_type BaseFace, texture::size_type MaxFace,
		texture::size_type BaseLevel, texture::size_type MaxLevel
	)
	{
		GLI_ASSERT(BaseLayer >= 0 && BaseLayer <= MaxLayer && MaxLayer < Src.layers());
		GLI_ASSERT(BaseFace >= 0 && BaseFace <= MaxFace && MaxFace < Src.faces());
		GLI_ASSERT(BaseLevel >= 0 && BaseLevel <= MaxLevel && MaxLevel < Src.levels());

		texture::size_type LevelsSize = 0;
		for(texture::size_type LevelIndex = 0; LevelIndex < MaxLevel - BaseLevel + 1; ++LevelIndex)
		{
			GLI_ASSERT(Dst.size(LevelIndex) == Src.size(LevelIndex));
			LevelsSize += Dst.size(LevelIndex);
		}

		for(texture::size_type LayerIndex = 0, LayerCount = MaxLayer - BaseLayer + 1; LayerIndex < LayerCount; ++LayerIndex)
		for(texture::size_type FaceIndex = 0, FaceCount = MaxFace - BaseFace + 1; FaceIndex < FaceCount; ++FaceIndex)
		{
			memcpy(Dst.data(LayerIndex, FaceIndex, BaseLevel), Src.data(BaseLayer + LayerIndex, BaseFace + FaceIndex, BaseLevel), LevelsSize);
		}
	}
}//namespace detail

	inline image duplicate(image const & Image)
	{
		image Result(Image.format(), Image.extent());

		memcpy(Result.data(), Image.data(), Image.size());
		
		return Result;
	}

	template <>
	inline texture duplicate(texture const & Texture)
	{
		texture Duplicate(
			Texture.target(),
			Texture.format(),
			Texture.extent(),
			Texture.layers(),
			Texture.faces(),
			Texture.levels());

		detail::duplicate_images(
			Texture, Duplicate,
			0, Texture.layers() - 1,
			0, Texture.faces() - 1,
			0, Texture.levels() - 1);

		return Duplicate;
	}

	template <typename texType>
	inline texture duplicate(texType const & Texture)
	{
		texture Duplicate(
			Texture.target(),
			Texture.format(),
			Texture.texture::extent(),
			Texture.layers(),
			Texture.faces(),
			Texture.levels());

		detail::duplicate_images(
			Texture, Duplicate,
			0, Texture.layers() - 1,
			0, Texture.faces() - 1,
			0, Texture.levels() - 1);

		return Duplicate;
	}

	template <typename texType>
	inline texture duplicate(texType const & Texture, typename texType::format_type Format)
	{
		GLI_ASSERT(block_size(Texture.format()) == block_size(Format));

		texture Duplicate(
			Texture.target(),
			Format,
			Texture.extent(),
			Texture.layers(),
			Texture.faces(),
			Texture.levels());

		detail::duplicate_images(
			Texture, Duplicate,
			0, Texture.layers() - 1,
			0, Texture.faces() - 1,
			0, Texture.levels() - 1);

		return Duplicate;
	}

	inline texture duplicate
	(
		texture1D const & Texture,
		texture1D::size_type BaseLevel, texture1D::size_type MaxLevel
	)
	{
		GLI_ASSERT(BaseLevel <= MaxLevel);
		GLI_ASSERT(BaseLevel < Texture.levels());
		GLI_ASSERT(MaxLevel < Texture.levels());
	
		texture1D Duplicate(
			Texture.format(),
			Texture.extent(BaseLevel),
			MaxLevel - BaseLevel + 1);

		memcpy(Duplicate.data(), Texture.data(0, 0, BaseLevel), Duplicate.size());

		return Duplicate;
	}

	inline texture duplicate
	(
		texture1DArray const & Texture,
		texture1DArray::size_type BaseLayer, texture1DArray::size_type MaxMayer,
		texture1DArray::size_type BaseLevel, texture1DArray::size_type MaxLevel
	)
	{
		GLI_ASSERT(BaseLevel <= MaxLevel);
		GLI_ASSERT(BaseLevel < Texture.levels());
		GLI_ASSERT(MaxLevel < Texture.levels());
		GLI_ASSERT(BaseLayer <= MaxMayer);
		GLI_ASSERT(BaseLayer < Texture.layers());
		GLI_ASSERT(MaxMayer < Texture.layers());

		texture1DArray Duplicate(
			Texture.format(),
			Texture[BaseLayer].extent(BaseLevel),
			MaxMayer - BaseLayer + 1,
			MaxLevel - BaseLevel + 1);

		for(texture1DArray::size_type Layer = 0; Layer < Duplicate.layers(); ++Layer)
			memcpy(Duplicate.data(Layer, 0, 0), Texture.data(Layer + BaseLayer, 0, BaseLevel), Duplicate[Layer].size());

		return Duplicate;
	}

	inline texture duplicate
	(
		texture2D const & Texture,
		texture2D::size_type BaseLevel, texture2D::size_type MaxLevel
	)
	{
		GLI_ASSERT(BaseLevel <= MaxLevel);
		GLI_ASSERT(BaseLevel < Texture.levels());
		GLI_ASSERT(MaxLevel < Texture.levels());
	
		texture2D Duplicate(
			Texture.format(),
			Texture.extent(BaseLevel),
			MaxLevel - BaseLevel + 1);

		memcpy(Duplicate.data(), Texture.data(0, 0, BaseLevel), Duplicate.size());

		return Duplicate;
	}

	inline texture duplicate
	(
		texture2DArray const & Texture,
		texture2DArray::size_type BaseLayer, texture2DArray::size_type MaxMayer,
		texture2DArray::size_type BaseLevel, texture2DArray::size_type MaxLevel
	)
	{
		GLI_ASSERT(BaseLevel <= MaxLevel);
		GLI_ASSERT(BaseLevel < Texture.levels());
		GLI_ASSERT(MaxLevel < Texture.levels());
		GLI_ASSERT(BaseLayer <= MaxMayer);
		GLI_ASSERT(BaseLayer < Texture.layers());
		GLI_ASSERT(MaxMayer < Texture.layers());

		texture2DArray Duplicate(
			Texture.format(),
			Texture.extent(BaseLevel),
			MaxMayer - BaseLayer + 1,
			MaxLevel - BaseLevel + 1);

		for(texture2DArray::size_type Layer = 0; Layer < Duplicate.layers(); ++Layer)
			memcpy(Duplicate.data(Layer, 0, 0), Texture.data(Layer + BaseLayer, 0, BaseLevel), Duplicate[Layer].size());

		return Duplicate;
	}

	inline texture duplicate
	(
		texture3D const & Texture,
		texture3D::size_type BaseLevel, texture3D::size_type MaxLevel
	)
	{
		GLI_ASSERT(BaseLevel <= MaxLevel);
		GLI_ASSERT(BaseLevel < Texture.levels());
		GLI_ASSERT(MaxLevel < Texture.levels());

		texture3D Duplicate(
			Texture.format(),
			Texture.extent(BaseLevel),
			MaxLevel - BaseLevel + 1);

		memcpy(Duplicate.data(), Texture.data(0, 0, BaseLevel), Duplicate.size());

		return Duplicate;
	}

	inline texture duplicate
	(
		textureCube const & Texture,
		textureCube::size_type BaseFace, textureCube::size_type MaxFace,
		textureCube::size_type BaseLevel, textureCube::size_type MaxLevel
	)
	{
		GLI_ASSERT(BaseLevel >= 0 && BaseLevel < Texture.levels() && BaseLevel <= MaxLevel && MaxLevel < Texture.levels());
		GLI_ASSERT(BaseFace <= MaxFace);
		GLI_ASSERT(BaseFace < Texture.faces());
		GLI_ASSERT(MaxFace < Texture.faces());

		textureCube Duplicate(
			Texture.format(),
			Texture[BaseFace].extent(BaseLevel),
			MaxLevel - BaseLevel + 1);

		for(textureCube::size_type Face = 0; Face < Duplicate.faces(); ++Face)
			memcpy(Duplicate[Face].data(), Texture[Face + BaseFace][BaseLevel].data(), Duplicate[Face].size());

		return Duplicate;
	}

	inline texture duplicate
	(
		textureCubeArray const & Texture,
		textureCubeArray::size_type BaseLayer, textureCubeArray::size_type MaxLayer,
		textureCubeArray::size_type BaseFace, textureCubeArray::size_type MaxFace,
		textureCubeArray::size_type BaseLevel, textureCubeArray::size_type MaxLevel
	)
	{
		GLI_ASSERT(BaseLevel <= MaxLevel);
		GLI_ASSERT(BaseLevel < Texture.levels());
		GLI_ASSERT(MaxLevel < Texture.levels());
		GLI_ASSERT(BaseFace <= MaxFace);
		GLI_ASSERT(BaseFace < Texture.faces());
		GLI_ASSERT(MaxFace < Texture.faces());
		GLI_ASSERT(BaseLayer <= MaxLayer);
		GLI_ASSERT(BaseLayer < Texture.layers());
		GLI_ASSERT(MaxLayer < Texture.layers());

		textureCubeArray Duplicate(
			Texture.format(),
			Texture[BaseLayer][BaseFace].extent(BaseLevel),
			MaxLayer - BaseLayer + 1,
			MaxLevel - BaseLevel + 1);

		for(textureCubeArray::size_type Layer = 0; Layer < Duplicate.layers(); ++Layer)
		for(textureCubeArray::size_type Face = 0; Face < Duplicate[Layer].faces(); ++Face)
			memcpy(Duplicate[Layer][Face].data(), Texture[Layer + BaseLayer][Face + BaseFace][BaseLevel].data(), Duplicate[Layer][Face].size());

		return Duplicate;
	}
}//namespace gli
