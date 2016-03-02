namespace gli
{
	inline texture_cube_array::texture_cube_array()
	{}

	inline texture_cube_array::texture_cube_array(format_type Format, extent_type const& Extent, size_type Layers, swizzles_type const& Swizzles)
		: texture(TARGET_CUBE_ARRAY, Format, texture::extent_type(Extent, 1), Layers, 6, gli::levels(Extent), Swizzles)
	{
		this->build_cache();
	}

	inline texture_cube_array::texture_cube_array(format_type Format, extent_type const& Extent, size_type Layers, size_type Levels, swizzles_type const& Swizzles)
		: texture(TARGET_CUBE_ARRAY, Format, texture::extent_type(Extent, 1), Layers, 6, Levels, Swizzles)
	{
		this->build_cache();
	}

	inline texture_cube_array::texture_cube_array(texture const & Texture)
		: texture(Texture, gli::TARGET_CUBE_ARRAY, Texture.format())
	{
		this->build_cache();
	}

	inline texture_cube_array::texture_cube_array
	(
		texture const & Texture,
		format_type Format,
		size_type BaseLayer, size_type MaxLayer,
		size_type BaseFace, size_type MaxFace,
		size_type BaseLevel, size_type MaxLevel,
		swizzles_type const& Swizzles
	)
		: texture(
			Texture, gli::TARGET_CUBE_ARRAY,
			Format,
			BaseLayer, MaxLayer,
			BaseFace, MaxFace,
			BaseLevel, MaxLevel,
			Swizzles)
	{
		this->build_cache();
	}

	inline texture_cube_array::texture_cube_array
	(
		texture_cube_array const & Texture,
		size_type BaseLayer, size_type MaxLayer,
		size_type BaseFace, size_type MaxFace,
		size_type BaseLevel, size_type MaxLevel
	)
		: texture(
			Texture, gli::TARGET_CUBE_ARRAY, Texture.format(),
			Texture.base_layer() + BaseLayer, Texture.base_layer() + MaxLayer,
			Texture.base_face() + BaseFace, Texture.base_face() + MaxFace,
			Texture.base_level() + BaseLevel, Texture.base_level() + MaxLevel)
	{
		this->build_cache();
	}

	inline texture_cube texture_cube_array::operator[](size_type Layer) const
	{
		GLI_ASSERT(Layer < this->layers());

		return texture_cube(
			*this, this->format(),
			this->base_layer() + Layer, this->base_layer() + Layer,
			this->base_face(), this->max_face(),
			this->base_level(), this->max_level());
	}

	inline texture_cube_array::extent_type texture_cube_array::extent(size_type Level) const
	{
		GLI_ASSERT(!this->empty());

		return this->Caches[this->index_cache(0, 0, Level)].ImageExtent;
	}

	template <typename gen_type>
	inline gen_type texture_cube_array::load(extent_type const& TexelCoord, size_type Layer,  size_type Face, size_type Level) const
	{
		GLI_ASSERT(!this->empty());
		GLI_ASSERT(!is_compressed(this->format()));
		GLI_ASSERT(block_size(this->format()) == sizeof(gen_type));

		cache const& Cache = this->Caches[this->index_cache(Layer, Face, Level)];

		size_type const ImageOffset = this->Storage->image_offset(TexelCoord, Cache.ImageExtent);
		GLI_ASSERT(ImageOffset < Cache.ImageMemorySize / sizeof(gen_type));

		return *(this->data<gen_type>(Layer, Face, Level) + ImageOffset);
	}

	template <typename gen_type>
	inline void texture_cube_array::store(extent_type const& TexelCoord, size_type Layer,  size_type Face, size_type Level, gen_type const& Texel)
	{
		GLI_ASSERT(!this->empty());
		GLI_ASSERT(!is_compressed(this->format()));
		GLI_ASSERT(block_size(this->format()) == sizeof(gen_type));

		cache const& Cache = this->Caches[this->index_cache(Layer, Face, Level)];
		GLI_ASSERT(glm::all(glm::lessThan(TexelCoord, Cache.ImageExtent)));

		size_type const ImageOffset = this->Storage->image_offset(TexelCoord, Cache.ImageExtent);
		GLI_ASSERT(ImageOffset < Cache.ImageMemorySize / sizeof(gen_type));

		*(this->data<gen_type>(Layer, Face, Level) + ImageOffset) = Texel;
	}

	inline texture_cube_array::size_type texture_cube_array::index_cache(size_type Layer, size_type Face, size_type Level) const
	{
		return (Layer * this->levels() * this->faces()) + (Face * this->levels()) + Level;
	}

	inline void texture_cube_array::build_cache()
	{
		this->Caches.resize(this->layers() * this->faces() * this->levels());

		for(size_type Layer = 0; Layer < this->layers(); ++Layer)
		for(size_type Face = 0; Face < this->faces(); ++Face)
		for(size_type Level = 0, Levels = this->levels(); Level < Levels; ++Level)
		{
			cache& Cache = this->Caches[this->index_cache(Layer, Face, Level)];
			Cache.ImageExtent = glm::max(texture2d::extent_type(this->texture::extent(Level)), texture2d::extent_type(1));
#			ifndef NDEBUG
				Cache.ImageMemorySize = this->size(Level);
#			endif
		}
	}
}//namespace gli
