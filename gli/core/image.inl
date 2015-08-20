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
/// @file gli/core/image.inl
/// @date 2011-10-06 / 2013-01-12
/// @author Christophe Riccio
///////////////////////////////////////////////////////////////////////////////////

namespace gli
{
	inline image::image()
		: BaseLayer(0)
		, BaseFace(0)
		, BaseLevel(0)
	{}

	inline image::image
	(
		format const & Format,
		dim_type const & Dimensions
	)
		: Storage(1, 1, 1, Format, dim_type(Dimensions))
		, BaseLayer(0)
		, BaseFace(0)
		, BaseLevel(0)
	{}

	inline image::image
	(
		storage const & Storage,
		size_type BaseLayer,
		size_type BaseFace,
		size_type BaseLevel
	)
		: Storage(Storage)
		, BaseLayer(BaseLayer)
		, BaseFace(BaseFace)
		, BaseLevel(BaseLevel)
	{}

	inline bool image::empty() const
	{
		return this->Storage.empty();
	}

	inline image::size_type image::size() const
	{
		assert(!this->empty());

		return this->Storage.level_size(this->BaseLevel);
	}

	template <typename genType>
	inline image::size_type image::size() const
	{
		assert(sizeof(genType) <= block_size(this->Storage.format()));

		return this->size() / sizeof(genType);
	}

	inline image::dim_type image::dimensions() const
	{
		return image::dim_type(this->Storage.dimensions(this->BaseLevel));
	}

	inline void * image::data()
	{
		assert(!this->empty());

		size_type const offset = this->Storage.addressing(
			this->BaseLayer, this->BaseFace, this->BaseLevel);

		return this->Storage.data() + offset;
	}

	inline void const * image::data() const
	{
		assert(!this->empty());
		
		size_type const offset = this->Storage.addressing(
			this->BaseLayer, this->BaseFace, this->BaseLevel);

		return this->Storage.data() + offset;
	}

	template <typename genType>
	inline genType * image::data()
	{
		assert(!this->empty());
		assert(block_size(this->Storage.format()) >= sizeof(genType));

		return reinterpret_cast<genType *>(this->data());
	}

	template <typename genType>
	inline genType const * image::data() const
	{
		assert(!this->empty());
		assert(block_size(this->Storage.format()) >= sizeof(genType));

		return reinterpret_cast<genType const *>(this->data());
	}

	inline void image::clear()
	{
		assert(!this->empty());

		memset(this->data<glm::byte>(), 0, this->size<glm::byte>());
	}

	template <typename genType>
	inline void image::clear(genType const & Texel)
	{
		assert(!this->empty());
		assert(block_size(this->Storage.format()) == sizeof(genType));

		for(size_type TexelIndex = 0; TexelIndex < this->size<genType>(); ++TexelIndex)
			*(this->data<genType>() + TexelIndex) = Texel;
	}

	template <typename genType>
	genType image::load(dim_type const & TexelCoord)
	{
		assert(!this->empty());
		assert(block_size(this->Storage.format()) == sizeof(genType));

		return *(this->data<genType>() + detail::texelLinearAdressing(this->dimensions(), TexelCoord));
	}

	template <typename genType>
	void image::store(dim_type const & TexelCoord, genType const & Data)
	{
		assert(!this->empty());
		assert(block_size(this->Storage.format()) == sizeof(genType));

		*(this->data<genType>() + detail::texelLinearAdressing(this->dimensions(), TexelCoord)) = Data;
	}
}//namespace gli
