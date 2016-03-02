#include "../levels.hpp"

namespace gli
{
	inline texture3d::texture3d()
	{}

	inline texture3d::texture3d(format_type Format, extent_type const& Extent, swizzles_type const& Swizzles)
		: texture(TARGET_3D, Format, Extent, 1, 1, gli::levels(Extent), Swizzles)
	{}

	inline texture3d::texture3d(format_type Format, extent_type const& Extent, size_type Levels, swizzles_type const& Swizzles)
		: texture(TARGET_3D, Format, Extent, 1, 1, Levels, Swizzles)
	{}

	inline texture3d::texture3d(texture const& Texture)
		: texture(Texture, TARGET_3D, Texture.format())
	{}

	inline texture3d::texture3d
	(
		texture const& Texture,
		format_type Format,
		size_type BaseLayer, size_type MaxLayer,
		size_type BaseFace, size_type MaxFace,
		size_type BaseLevel, size_type MaxLevel,
		swizzles_type const& Swizzles
	)
		: texture(
			Texture, TARGET_3D, Format,
			BaseLayer, MaxLayer,
			BaseFace, MaxFace,
			BaseLevel, MaxLevel,
			Swizzles)
	{}
 
	inline texture3d::texture3d
	(
		texture3d const& Texture,
		size_type BaseLevel, size_type MaxLevel
	)
		: texture(
			Texture, TARGET_3D, Texture.format(),
			Texture.base_layer(), Texture.max_layer(),
			Texture.base_face(), Texture.max_face(),
			Texture.base_level() + BaseLevel, Texture.base_level() + MaxLevel)
	{}

	inline image texture3d::operator[](size_type Level) const
	{
		GLI_ASSERT(Level < this->levels());

		return image(
			this->Storage,
			this->format(),
			this->base_layer(),
			this->base_face(),
			this->base_level() + Level);
	}

	inline texture3d::extent_type texture3d::extent(size_type Level) const
	{
		return extent_type(this->texture::extent(Level));
	}

	template <typename gen_type>
	inline gen_type texture3d::load(extent_type const& TexelCoord, size_type Level) const
	{
		GLI_ASSERT(!this->empty());
		GLI_ASSERT(!is_compressed(this->format()));
		GLI_ASSERT(block_size(this->format()) == sizeof(gen_type));

		size_type const ImageOffset = this->Storage->image_offset(TexelCoord, this->texture::extent(Level));
		GLI_ASSERT(ImageOffset < this->size<gen_type>(Level));

		return *(this->data<gen_type>(0, 0, Level) + ImageOffset);
	}

	template <typename gen_type>
	inline void texture3d::store(extent_type const& TexelCoord, size_type Level, gen_type const& Texel)
	{
		GLI_ASSERT(!this->empty());
		GLI_ASSERT(!is_compressed(this->format()));
		GLI_ASSERT(block_size(this->format()) == sizeof(gen_type));
		GLI_ASSERT(glm::all(glm::lessThan(TexelCoord, this->texture::extent(Level))));

		size_type const ImageOffset = this->Storage->image_offset(TexelCoord, this->texture::extent(Level));
		GLI_ASSERT(ImageOffset < this->size<gen_type>(Level));

		*(this->data<gen_type>(0, 0, Level) + ImageOffset) = Texel;
	}
}//namespace gli
