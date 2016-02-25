#include "../levels.hpp"

namespace gli
{
	inline texture2d::texture2d()
	{}

	inline texture2d::texture2d(format_type Format, extent_type const& Extent, swizzles_type const& Swizzles)
		: texture(TARGET_2D, Format, texture::extent_type(Extent, 1), 1, 1, gli::levels(Extent), Swizzles)
	{
		this->build_cache();
	}

	inline texture2d::texture2d(format_type Format, extent_type const& Extent, size_type Levels, swizzles_type const& Swizzles)
		: texture(TARGET_2D, Format, texture::extent_type(Extent, 1), 1, 1, Levels, Swizzles)
	{
		this->build_cache();
	}

	inline texture2d::texture2d(texture const& Texture)
		: texture(Texture, TARGET_2D, Texture.format())
	{
		this->build_cache();
	}

	inline texture2d::texture2d
	(
		texture const& Texture,
		format_type Format,
		size_type BaseLayer, size_type MaxLayer,
		size_type BaseFace, size_type MaxFace,
		size_type BaseLevel, size_type MaxLevel,
		swizzles_type const& Swizzles
	)
		: texture(
			Texture, TARGET_2D, Format,
			BaseLayer, MaxLayer,
			BaseFace, MaxFace,
			BaseLevel, MaxLevel,
			Swizzles)
	{
		this->build_cache();
	}

	inline texture2d::texture2d
	(
		texture2d const& Texture,
		size_type BaseLevel, size_type MaxLevel
	)
		: texture(
			Texture, TARGET_2D, Texture.format(),
			Texture.base_layer(), Texture.max_layer(),
			Texture.base_face(), Texture.max_face(),
			Texture.base_level() + BaseLevel, Texture.base_level() + MaxLevel)
	{
		this->build_cache();
	}

	inline image texture2d::operator[](size_type Level) const
	{
		GLI_ASSERT(Level < this->levels());

		return image(
			this->Storage,
			this->format(),
			this->base_layer(),
			this->base_face(),
			this->base_level() + Level);
	}

	inline texture2d::extent_type texture2d::extent(size_type Level) const
	{
		GLI_ASSERT(!this->empty());

		return this->Caches[this->index_cache(Level)].ImageExtent;
	}

	template <typename gen_type>
	inline gen_type texture2d::load(extent_type const& TexelCoord, size_type Level) const
	{
		GLI_ASSERT(!this->empty());
		GLI_ASSERT(!is_compressed(this->format()));
		GLI_ASSERT(block_size(this->format()) == sizeof(gen_type));

		cache const& Cache = this->Caches[this->index_cache(Level)];

		size_type const ImageOffset = this->Storage->image_offset(TexelCoord, Cache.ImageExtent);
		GLI_ASSERT(ImageOffset < Cache.ImageMemorySize / sizeof(gen_type));

		return *(reinterpret_cast<gen_type const* const>(Cache.ImageBaseAddress) + ImageOffset);
	}

	template <typename gen_type>
	inline void texture2d::store(extent_type const& TexelCoord, size_type Level, gen_type const& Texel)
	{
		GLI_ASSERT(!this->empty());
		GLI_ASSERT(!is_compressed(this->format()));
		GLI_ASSERT(block_size(this->format()) == sizeof(gen_type));

		cache const& Cache = this->Caches[this->index_cache(Level)];
		GLI_ASSERT(glm::all(glm::lessThan(TexelCoord, Cache.ImageExtent)));

		size_type const ImageOffset = this->Storage->image_offset(TexelCoord, Cache.ImageExtent);
		GLI_ASSERT(ImageOffset < Cache.ImageMemorySize / sizeof(gen_type));

		*(reinterpret_cast<gen_type* const>(Cache.ImageBaseAddress) + ImageOffset) = Texel;
	}

	inline void texture2d::clear()
	{
		this->texture::clear();
	}

	template <typename gen_type>
	inline void texture2d::clear(gen_type const& Texel)
	{
		this->texture::clear<gen_type>(Texel);
	}

	template <typename gen_type>
	inline void texture2d::clear(size_type Level, gen_type const& Texel)
	{
		this->texture::clear<gen_type>(0, 0, Level, Texel);
	}

	inline texture2d::size_type texture2d::index_cache(size_type Level) const
	{
		return Level;
	}

	inline void texture2d::build_cache()
	{
		this->Caches.resize(this->levels());

		for(size_type Level = 0, Levels = this->levels(); Level < Levels; ++Level)
		{
			cache& Cache = this->Caches[this->index_cache(Level)];
			Cache.ImageBaseAddress = this->data<std::uint8_t>(0, 0, Level);
			Cache.ImageExtent = glm::max(extent_type(this->texture::extent(Level)), extent_type(1));
#			ifndef NDEBUG
				Cache.ImageMemorySize = this->size(Level);
#			endif
		}
	}
}//namespace gli
