///////////////////////////////////////////////////////////////////////////////////
/// OpenGL Image (gli.g-truc.net)
///
/// Copyright (c) 2008 - 2015 G-Truc Creation (www.g-truc.net)
/// Permission is hereby granted, free of charge, to any person obtaining a copy
/// of this software and associated documentation files (the "Software"), to deal
/// in the Software without restriction, including without limitation the rights
/// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
/// copies of the Software, and to permit persons to whom the Software is
/// furnished to do so, subject to the following conditions:
///
/// The above copyright notice and this permission notice shall be included in
/// all copies or substantial portions of the Software.
///
/// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
/// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
/// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
/// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
/// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
/// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
/// THE SOFTWARE.
///
/// @ref core
/// @file gli/core/texture3d.inl
/// @date 2013-01-13 / 2013-01-13
/// @author Christophe Riccio
///////////////////////////////////////////////////////////////////////////////////

#include "../levels.hpp"

namespace gli
{
	inline texture3D::texture3D()
	{}

	inline texture3D::texture3D(format_type Format, dim_type const & Dimensions)
		: texture(gli::TARGET_3D, Format, Dimensions, 1, 1, gli::levels(Dimensions))
		, Caches(this->levels())
	{
		this->build_cache();
	}

	inline texture3D::texture3D(format_type Format, dim_type const & Dimensions, size_type Levels)
		: texture(gli::TARGET_3D, Format, Dimensions, 1, 1, Levels)
		, Caches(this->levels())
	{
		this->build_cache();
	}

	inline texture3D::texture3D(texture const & Texture)
		: texture(Texture, gli::TARGET_3D, Texture.format())
		, Caches(this->levels())
	{
		this->build_cache();
	}

	inline texture3D::texture3D
	(
		texture const & Texture,
		format_type Format,
		size_type BaseLayer, size_type MaxLayer,
		size_type BaseFace, size_type MaxFace,
		size_type BaseLevel, size_type MaxLevel
	)
		: texture(
			Texture, gli::TARGET_3D, Format,
			BaseLayer, MaxLayer,
			BaseFace, MaxFace,
			BaseLevel, MaxLevel)
		, Caches(this->levels())
	{
		this->build_cache();
	}
 
	inline texture3D::texture3D
	(
		texture3D const & Texture,
		size_type BaseLevel, size_type MaxLevel
	)
		: texture(
			Texture, gli::TARGET_3D, Texture.format(),
			Texture.base_layer(), Texture.max_layer(),
			Texture.base_face(), Texture.max_face(),
			Texture.base_level() + BaseLevel, Texture.base_level() + MaxLevel)
		, Caches(this->levels())
	{
		this->build_cache();
	}

	inline image texture3D::operator[](texture3D::size_type Level) const
	{
		assert(Level < this->levels());

		return image(
			this->Storage,
			this->format(),
			this->base_layer(),
			this->base_face(),
			this->base_level() + Level);
	}

	inline texture3D::dim_type texture3D::dimensions(size_type Level) const
	{
		assert(!this->empty());

		return this->Caches[Level].Dim;
	}

	template <typename genType>
	inline genType texture3D::fetch(texture3D::dim_type const & TexelCoord, texture3D::size_type Level) const
	{
		assert(!this->empty());
		assert(!is_compressed(this->format()));
		assert(block_size(this->format()) == sizeof(genType));

		std::size_t const Index = TexelCoord.x + TexelCoord.y * this->Caches[Level].Dim.x;
		assert(Index < this->Caches[Level].Size / sizeof(genType));

		return reinterpret_cast<genType const * const>(this->Caches[Level].Data)[Index];
	}

	template <typename genType>
	void texture3D::write(texture3D::dim_type const & TexelCoord, texture3D::size_type Level, genType const & Color)
	{
		assert(!this->empty());
		assert(!is_compressed(this->format()));
		assert(block_size(this->format()) == sizeof(genType));
		assert(glm::all(glm::lessThan(TexelCoord, this->Caches[Level].Dim)));

		std::size_t const Index = TexelCoord.x + TexelCoord.y * this->Caches[Level].Dim.x;
		assert(Index < this->Caches[Level].Size / sizeof(genType));

		reinterpret_cast<genType*>(this->Caches[Level].Data)[Index] = Color;
	}

	void texture3D::build_cache()
	{
		for(size_type Level = 0; Level < this->levels(); ++Level)
		{
			this->Caches[Level].Data = this->data<std::uint8_t>(0, 0, Level);
			this->Caches[Level].Size = this->size(Level);
			this->Caches[Level].Dim = glm::max(texture3D::dim_type(this->texture::dimensions(Level)), texture3D::dim_type(1));
		}
	}
}//namespace gli
