///////////////////////////////////////////////////////////////////////////////////
/// OpenGL Image (gli.g-truc.net)
///
/// Copyright (c) 2008 - 2013 G-Truc Creation (www.g-truc.net)
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
/// @file gli/core/view.inl
/// @date 2013-02-03 / 2013-02-05
/// @author Christophe Riccio
///////////////////////////////////////////////////////////////////////////////////

namespace gli
{
	// texture can be texture1D, texture2D, texture3D
	template <typename texture>
	inline texture reference
	(
		texture const & Texture,
		typename texture::size_type const & BaseLevel,
		typename texture::size_type const & MaxLevel
	)
	{
		assert(detail::getFormatInfo(Format).BlockSize == detail::getFormatInfo(Texture.format()).BlockSize);
		assert(BaseLevel <= MaxLevel);
		assert(Texture.view().BaseFace <= BaseLevel);
		assert(Texture.view().MaxFace >= MaxLevel);

		return texture(
			Texture,
			Texture.format(),
			view(
				Texture.view().BaseLayer,
				Texture.view().MaxLayer,
				Texture.view().BaseFace,
				Texture.view().MaxFace,
				Texture.view().BaseLevel + BaseLevel,
				Texture.view().BaseLevel + MaxLevel));
	}

	// texture can be texture1DArray, texture2DArray
	template <typename texture>
	inline texture reference
	(
		texture const & Texture,
		typename texture::size_type const & BaseLayer,
		typename texture::size_type const & MaxLayer,
		typename texture::size_type const & BaseLevel,
		typename texture::size_type const & MaxLevel
	)
	{
		return texture(
			Texture,
			Texture.format(),
			view(
				Texture.view().BaseLayer + BaseLayer,
				Texture.view().BaseLayer + MaxLayer,
				Texture.view().BaseFace,
				Texture.view().MaxFace,
				Texture.view().BaseLevel + BaseLevel,
				Texture.view().BaseLevel + MaxLevel));
	}

	inline textureCube reference
	(
		textureCube const & Texture,
		textureCube::size_type const & BaseFace,
		textureCube::size_type const & MaxFace,
		textureCube::size_type const & BaseLevel,
		textureCube::size_type const & MaxLevel
	)
	{
		return textureCube(
			Texture,
			Texture.format(),
			view(
				Texture.view().BaseLayer,
				Texture.view().MaxLayer,
				Texture.view().BaseFace + BaseFace,
				Texture.view().BaseFace + MaxFace,
				Texture.view().BaseLevel + BaseLevel,
				Texture.view().BaseLevel + MaxLevel));
	}

	inline textureCubeArray reference
	(
		textureCubeArray const & Texture,
		textureCubeArray::size_type const & BaseLayer,
		textureCubeArray::size_type const & MaxLayer,
		textureCubeArray::size_type const & BaseFace,
		textureCubeArray::size_type const & MaxFace,
		textureCubeArray::size_type const & BaseLevel,
		textureCubeArray::size_type const & MaxLevel
	)
	{
		return textureCubeArray(
			Texture,
			Texture.format(),
			view(
				Texture.view().BaseLayer + BaseLayer,
				Texture.view().BaseLayer + MaxLayer,
				Texture.view().BaseFace + BaseFace,
				Texture.view().BaseFace + MaxFace,
				Texture.view().BaseLevel + BaseLevel,
				Texture.view().BaseLevel + MaxLevel));
	}

	template <typename texture>
	inline texture1D view1D
	(
		texture const & Texture,
		typename texture::format_type const & Format,
		typename texture::size_type const & BaseLayer,
		typename texture::size_type const & BaseFace,
		typename texture::size_type const & BaseLevel,
		typename texture::size_type const & MaxLevel
	)
	{
		assert(detail::getFormatInfo(Format).BlockSize == detail::getFormatInfo(Texture.format()).BlockSize);
		
		view const & View = Texture.view();

		return texture1D(
			Texture,
			Format,
			view(
				View.BaseLayer + BaseLayer,
				View.BaseLayer + BaseLayer,
				View.BaseFace + BaseFace,
				View.BaseFace + BaseFace,
				View.BaseLevel + BaseLevel,
				View.BaseLevel + MaxLevel));
	}

	template <typename texture>
	inline texture1DArray view1DArray
	(
		texture const & Texture,
		typename texture::format_type const & Format,
		typename texture::size_type const & BaseLayer,
		typename texture::size_type const & MaxLayer,
		typename texture::size_type const & BaseFace,
		typename texture::size_type const & BaseLevel,
		typename texture::size_type const & MaxLevel
	)
	{
		assert(detail::getFormatInfo(Format).BlockSize == detail::getFormatInfo(Texture.format()).BlockSize);
		assert(MaxLayer < Texture.layers());
		assert(MaxLevel < Texture.levels());

		view const & View = Texture.view();

		return texture1DArray(
			Texture,
			Format,
			view(
				View.BaseLayer + BaseLayer,
				View.BaseLayer + MaxLayer,
				View.BaseFace + BaseFace,
				View.BaseFace + BaseFace,
				View.BaseLevel + BaseLevel,
				View.BaseLevel + MaxLevel));
	}

	template <typename texture>
	inline texture2D view2D
	(
		texture const & Texture,
		typename texture::format_type const & Format,
		typename texture::size_type const & BaseLayer,
		typename texture::size_type const & BaseFace,
		typename texture::size_type const & BaseLevel,
		typename texture::size_type const & MaxLevel
	)
	{
		assert(detail::getFormatInfo(Format).BlockSize == detail::getFormatInfo(Texture.format()).BlockSize);
		assert(MaxLevel < Texture.levels());

		view const & View = Texture.view();

		return texture2D(
			Texture,
			Format,
			view(
				View.BaseLayer + BaseLayer,
				View.BaseLayer + BaseLayer,
				View.BaseFace + BaseFace,
				View.BaseFace + BaseFace,
				View.BaseLevel + BaseLevel,
				View.BaseLevel + MaxLevel));
	}

	template <typename texture>
	inline texture2DArray view2DArray
	(
		texture const & Texture,
		typename texture::format_type const & Format,
		typename texture::size_type const & BaseLayer,
		typename texture::size_type const & MaxLayer,
		typename texture::size_type const & BaseFace,
		typename texture::size_type const & BaseLevel,
		typename texture::size_type const & MaxLevel
	)
	{
		assert(detail::getFormatInfo(Format).BlockSize == detail::getFormatInfo(Texture.format()).BlockSize);
		assert(MaxLayer < Texture.layers());
		assert(MaxLevel < Texture.levels());

		view const & View = Texture.view();

		return texture2DArray(
			Texture,
			Format,
			view(
				View.BaseLayer + BaseLayer,
				View.BaseLayer + MaxLayer,
				View.BaseFace + BaseFace,
				View.BaseFace + BaseFace,
				View.BaseLevel + BaseLevel,
				View.BaseLevel + MaxLevel));
	}

	template <typename texture>
	inline texture3D view3D
	(
		texture const & Texture,
		typename texture::format_type const & Format,
		typename texture::size_type const & BaseLayer,
		typename texture::size_type const & BaseFace,
		typename texture::size_type const & BaseLevel,
		typename texture::size_type const & MaxLevel
	)
	{
		assert(detail::getFormatInfo(Format).BlockSize == detail::getFormatInfo(Texture.format()).BlockSize);
		assert(MaxLevel < Texture.levels());

		view const & View = Texture.view();

		return texture3D(
			Texture,
			Format,
			view(
				View.BaseLayer + BaseLayer,
				View.BaseLayer + BaseLayer,
				View.BaseFace + BaseFace,
				View.BaseFace + BaseFace,
				View.BaseLevel + BaseLevel,
				View.BaseLevel + MaxLevel));
	}

	template <typename texture>
	inline textureCube viewCube
	(
		texture const & Texture,
		typename texture::format_type const & Format,
		typename texture::size_type const & BaseLayer,
		typename texture::size_type const & BaseFace,
		typename texture::size_type const & MaxFace,
		typename texture::size_type const & BaseLevel,
		typename texture::size_type const & MaxLevel
	)
	{
		assert(detail::getFormatInfo(Format).BlockSize == detail::getFormatInfo(Texture.format()).BlockSize);
		assert(MaxFace < Texture.faces());
		assert(MaxLevel < Texture.levels());

		view const & View = Texture.view();

		return textureCube(
			Texture,
			Format,
			view(
				View.BaseLayer + BaseLayer,
				View.BaseLayer + BaseLayer,
				View.BaseFace + BaseFace,
				View.BaseFace + MaxFace,
				View.BaseLevel + BaseLevel,
				View.BaseLevel + MaxLevel));
	}

	template <typename texture>
	inline textureCubeArray viewCubeArray
	(
		texture const & Texture,
		typename texture::format_type const & Format,
		typename texture::size_type const & BaseLayer,
		typename texture::size_type const & MaxLayer,
		typename texture::size_type const & BaseFace,
		typename texture::size_type const & MaxFace,
		typename texture::size_type const & BaseLevel,
		typename texture::size_type const & MaxLevel
	)
	{
		assert(detail::getFormatInfo(Format).BlockSize == detail::getFormatInfo(Texture.format()).BlockSize);
		assert(MaxLayer < Texture.layers());
		assert(MaxFace < Texture.faces());
		assert(MaxLevel < Texture.levels());

		view const & View = Texture.view();

		return textureCubeArray(
			Texture,
			Format,
			view(
				View.BaseLayer + BaseLayer,
				View.BaseLayer + MaxLayer,
				View.BaseFace + BaseFace,
				View.BaseFace + MaxFace,
				View.BaseLevel + BaseLevel,
				View.BaseLevel + MaxLevel));
	}
}//namespace gli
