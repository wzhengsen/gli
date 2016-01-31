#include "../levels.hpp"

namespace gli
{
	inline texture1D::texture1D()
	{}

	inline texture1D::texture1D(format_type Format, texelcoord_type const & Dimensions)
		: texture(TARGET_1D, Format, texture::texelcoord_type(Dimensions.x, 1, 1), 1, 1, gli::levels(Dimensions))
	{
		this->build_cache();
	}

	inline texture1D::texture1D(format_type Format, texelcoord_type const & Dimensions, size_type Levels)
		: texture(TARGET_1D, Format, texture::texelcoord_type(Dimensions.x, 1, 1), 1, 1, Levels)
	{
		this->build_cache();
	}

	inline texture1D::texture1D(texture const & Texture)
		: texture(Texture, TARGET_1D, Texture.format())
	{
		this->build_cache();
	}

	inline texture1D::texture1D
	(
		texture const & Texture,
		format_type Format,
		size_type BaseLayer, size_type MaxLayer,
		size_type BaseFace, size_type MaxFace,
		size_type BaseLevel, size_type MaxLevel
	)
		: texture(
			Texture, TARGET_1D,
			Format,
			BaseLayer, MaxLayer,
			BaseFace, MaxFace,
			BaseLevel, MaxLevel)
	{
		this->build_cache();
	}
 
	inline texture1D::texture1D
	(
		texture1D const & Texture,
		size_type BaseLevel, size_type MaxLevel
	)
		: texture(
			Texture, TARGET_1D,
			Texture.format(),
			Texture.base_layer(), Texture.max_layer(),
			Texture.base_face(), Texture.max_face(),
			Texture.base_level() + BaseLevel, Texture.base_level() + MaxLevel)
	{
		this->build_cache();
	}

	inline image texture1D::operator[](texture1D::size_type Level) const
	{
		GLI_ASSERT(Level < this->levels());

		return image(
			this->Storage,
			this->format(),
			this->base_layer(),
			this->base_face(),
			this->base_level() + Level);
	}

	inline texture1D::texelcoord_type texture1D::extent(size_type Level) const
	{
		GLI_ASSERT(!this->empty());

		return this->Caches[this->index_cache(Level)].Extent;
	}

	template <typename genType>
	inline genType texture1D::load(texelcoord_type const & TexelCoord, size_type Level) const
	{
		GLI_ASSERT(!this->empty());
		GLI_ASSERT(!is_compressed(this->format()));
		GLI_ASSERT(block_size(this->format()) == sizeof(genType));

		cache const & Cache = this->Caches[this->index_cache(Level)];

		std::size_t const Index = linear_index(TexelCoord, Cache.Extent);
		GLI_ASSERT(Index < Cache.Size / sizeof(genType));

		return reinterpret_cast<genType const * const>(Cache.Data)[Index];
	}

	template <typename genType>
	inline void texture1D::store(texelcoord_type const& TexelCoord, size_type Level, genType const& Texel)
	{
		GLI_ASSERT(!this->empty());
		GLI_ASSERT(!is_compressed(this->format()));
		GLI_ASSERT(block_size(this->format()) == sizeof(genType));

		cache const & Cache = this->Caches[this->index_cache(Level)];
		GLI_ASSERT(glm::all(glm::lessThan(TexelCoord, Cache.Extent)));

		std::size_t const Index = linear_index(TexelCoord, Cache.Extent);
		GLI_ASSERT(Index < Cache.Size / sizeof(genType));

		reinterpret_cast<genType*>(Cache.Data)[Index] = Texel;
	}

	inline void texture1D::clear()
	{
		this->texture::clear();
	}

	template <typename genType>
	inline void texture1D::clear(genType const& Texel)
	{
		this->texture::clear<genType>(Texel);
	}

	template <typename genType>
	inline void texture1D::clear(size_type Level, genType const& Texel)
	{
		this->texture::clear<genType>(0, 0, Level, Texel);
	}

	inline texture1D::size_type texture1D::index_cache(size_type Level) const
	{
		return Level;
	}

	inline void texture1D::build_cache()
	{
		this->Caches.resize(this->levels());

		for (size_type LevelIndex = 0, LevelCount = this->levels(); LevelIndex < LevelCount; ++LevelIndex)
		{
			cache& Cache = this->Caches[this->index_cache(LevelIndex)];
			Cache.Data = this->data<std::uint8_t>(0, 0, LevelIndex);
			Cache.Extent = glm::max(texelcoord_type(this->texture::extent(LevelIndex)), texelcoord_type(1));
#			ifndef NDEBUG
				Cache.Size = this->size(LevelIndex);
#			endif
		}
	}
}//namespace gli
