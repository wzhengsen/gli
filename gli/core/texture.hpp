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
/// @file gli/core/texture.hpp
/// @date 2013-02-05 / 2013-02-05
/// @author Christophe Riccio
///////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "image.hpp"
#include "target.hpp"

namespace gli
{
	class texture
	{
	public:
		typedef size_t size_type;
		typedef gli::format format_type;
		typedef storage::dim_type dim_type;
		typedef storage::data_type data_type;

		texture();

		texture(
			gli::target Target,
			size_type const & Layers,
			size_type const & Faces,
			size_type const & Levels,
			format_type const & Format,
			storage::dim_type const & Dimensions);

		texture(
			texture const & Texture,
			gli::target Target,
			format_type const & Format,
			size_type BaseLayer, size_type MaxLayer,
			size_type BaseFace, size_type MaxFace,
			size_type BaseLevel, size_type MaxLevel);

		texture(
			texture const & Texture,
			gli::target Target);

		virtual ~texture(){}

		bool empty() const;
		format_type format() const;
		
		size_type base_layer() const;
		size_type max_layer() const;
		size_type layers() const;

		size_type base_face() const;
		size_type max_face() const;
		size_type faces() const;

		size_type base_level() const;
		size_type max_level() const;
		size_type levels() const;

		dim_type dimensions(size_type Level = 0) const;

		size_type size() const;
		template <typename genType>
		size_type size() const;

		void * data();
		template <typename genType>
		genType * data();
		void const * data() const;
		template <typename genType>
		genType const * data() const;

		void clear();
		template <typename genType>
		void clear(genType const & Texel);

		gli::target target() const{return this->Target;}

		/// Compute the relative memory offset to access the data for a specific layer, face and level
		size_t offset(
			size_t const & Layer,
			size_t const & Face,
			size_t const & Level) const;

		size_t level_size(size_t const & Level) const;

	protected:
		storage Storage;
		gli::target const Target;
		format_type const Format;
		size_type const BaseLayer;
		size_type const MaxLayer;
		size_type const BaseFace;
		size_type const MaxFace;
		size_type const BaseLevel;
		size_type const MaxLevel;
		void * const Data;
		size_type const Size;

	private:
		void * const compute_data() const;
		size_type compute_size() const;
	};

}//namespace gli

#include "texture.inl"

