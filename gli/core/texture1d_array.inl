#include "../levels.hpp"

namespace gli
{
	inline texture1d_array::texture1d_array()
	{}

	inline texture1d_array::texture1d_array(format_type Format, extent_type const& Extent, size_type Layers, swizzles_type const& Swizzles)
		: texture(TARGET_1D_ARRAY, Format, texture::extent_type(Extent.x, 1, 1), Layers, 1, gli::levels(Extent), Swizzles)
	{
		this->build_cache();
	}

	inline texture1d_array::texture1d_array(format_type Format, extent_type const& Extent, size_type Layers, size_type Levels, swizzles_type const& Swizzles)
		: texture(TARGET_1D_ARRAY, Format, texture::extent_type(Extent.x, 1, 1), Layers, 1, Levels, Swizzles)
	{
		this->build_cache();
	}

	inline texture1d_array::texture1d_array(texture const& Texture)
		: texture(Texture, TARGET_1D_ARRAY, Texture.format())
	{
		this->build_cache();
	}

	inline texture1d_array::texture1d_array
	(
		texture const& Texture,
		format_type Format,
		size_type BaseLayer, size_type MaxLayer,
		size_type BaseFace, size_type MaxFace,
		size_type BaseLevel, size_type MaxLevel,
		swizzles_type const& Swizzles
	)
		: texture(
			Texture, TARGET_1D_ARRAY, Format,
			BaseLayer, MaxLayer,
			BaseFace, MaxFace,
			BaseLevel, MaxLevel,
			Swizzles)
	{
		this->build_cache();
	}

	inline texture1d_array::texture1d_array
	(
		texture1d_array const & Texture,
		size_type BaseLayer, size_type MaxLayer,
		size_type BaseLevel, size_type MaxLevel
	)
		: texture(
			Texture, TARGET_1D_ARRAY,
			Texture.format(),
			Texture.base_layer() + BaseLayer, Texture.base_layer() + MaxLayer,
			Texture.base_face(), Texture.max_face(),
			Texture.base_level() + BaseLevel, Texture.base_level() + MaxLevel)
	{
		this->build_cache();
	}

	inline texture1d texture1d_array::operator[](size_type Layer) const
	{
		GLI_ASSERT(!this->empty());
		GLI_ASSERT(Layer < this->layers());

		return texture1d(
			*this, this->format(),
			this->base_layer() + Layer, this->base_layer() + Layer,
			this->base_face(), 	this->max_face(),
			this->base_level(), this->max_level());
	}

	inline texture1d_array::extent_type texture1d_array::extent(size_type Level) const
	{
		GLI_ASSERT(!this->empty());

		return this->Caches[this->index_cache(0, Level)].ImageExtent;
	}

	template <typename gen_type>
	inline gen_type texture1d_array::load(extent_type const& TexelCoord, size_type Layer, size_type Level) const
	{
		GLI_ASSERT(!this->empty());
		GLI_ASSERT(!is_compressed(this->format()));
		GLI_ASSERT(block_size(this->format()) == sizeof(gen_type));

		cache const& Cache = this->Caches[this->index_cache(Layer, Level)];

		size_type const ImageOffset = this->Storage->image_offset(TexelCoord, Cache.ImageExtent);
		GLI_ASSERT(ImageOffset < this->size<gen_type>(Level));

		return *(this->data<gen_type>(Layer, 0, Level) + ImageOffset);
	}

	template <typename gen_type>
	inline void texture1d_array::store(extent_type const& TexelCoord, size_type Layer, size_type Level, gen_type const& Texel)
	{
		GLI_ASSERT(!this->empty());
		GLI_ASSERT(!is_compressed(this->format()));
		GLI_ASSERT(block_size(this->format()) == sizeof(gen_type));

		cache& Cache = this->Caches[this->index_cache(Layer, Level)];
		GLI_ASSERT(glm::all(glm::lessThan(TexelCoord, Cache.ImageExtent)));

		size_type const ImageOffset = this->Storage->image_offset(TexelCoord, Cache.ImageExtent);
		GLI_ASSERT(ImageOffset < this->size<gen_type>(Level));

		*(this->data<gen_type>(Layer, 0, Level) + ImageOffset) = Texel;
	}

	inline texture1d_array::size_type texture1d_array::index_cache(size_type Layer, size_type Level) const
	{
		return Layer * this->levels() + Level;
	}

	inline void texture1d_array::build_cache()
	{
		this->Caches.resize(this->layers() * this->levels());

		for(size_type Layer = 0; Layer < this->layers(); ++Layer)
		for(size_type Level = 0, Levels = this->levels(); Level < Levels; ++Level)
		{
			cache& Cache = this->Caches[this->index_cache(Layer, Level)];
			Cache.ImageExtent = glm::max(extent_type(this->texture::extent(Level)), extent_type(1));
		}
	}
}//namespace gli


