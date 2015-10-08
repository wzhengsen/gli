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
/// @file gli/core/convert.hpp
/// @date 2015-10-08 / 2015-10-08
/// @author Christophe Riccio
///////////////////////////////////////////////////////////////////////////////////

namespace gli{
namespace detail
{
	template <typename retType, typename T, gli::precision P, template <typename, gli::precision> class vecType, bool normalize = false, bool srgb = false>
	struct convert
	{
		static vecType<retType, P> fetch(gli::texture2D const & Texture, gli::texture2D::dim_type const & TexelCoord, gli::texture2D::size_type Level)
		{
			return vecType<retType, P>(Texture.load<vecType<T, P> >(TexelCoord, Level));
		}

		static void write(gli::texture2D & Texture, gli::texture2D::dim_type const & TexelCoord, gli::texture2D::size_type Level, vecType<retType, P> const & Texel)
		{
			Texture.store<vecType<T, P> >(TexelCoord, Level, vecType<T, P>(Texel));
		}
	};

	template <typename retType, typename T, gli::precision P, template <typename, gli::precision> class vecType>
	struct convert<retType, T, P, vecType, true, false>
	{
		static vecType<retType, P> fetch(gli::texture2D const & Texture, gli::texture2D::dim_type const & TexelCoord, gli::texture2D::size_type Level)
		{
			return gli::compNormalize<retType>(Texture.load<vecType<T, P> >(TexelCoord, Level));
		}

		static void write(gli::texture2D & Texture, gli::texture2D::dim_type const & TexelCoord, gli::texture2D::size_type Level, vecType<retType, P> const & Texel)
		{
			Texture.store<vecType<T, P> >(TexelCoord, Level, glm::compScale<T>(Texel));
		}
	};

	template <typename retType, typename T, gli::precision P, template <typename, gli::precision> class vecType>
	struct convert<retType, T, P, vecType, true, true>
	{
		static vecType<retType, P> fetch(gli::texture2D const & Texture, gli::texture2D::dim_type const & TexelCoord, gli::texture2D::size_type Level)
		{
			return gli::convertSRGBToLinear(gli::compNormalize<retType>(Texture.load<vecType<T, P> >(TexelCoord, Level)));
		}

		static void write(gli::texture2D & Texture, gli::texture2D::dim_type const & TexelCoord, gli::texture2D::size_type Level, vecType<retType, P> const & Texel)
		{
			Texture.store<vecType<T, P> >(TexelCoord, Level, gli::compScale<T>(gli::convertLinearToSRGB(Texel)));
		}
	};
}//namespace detail
}//namespace gli
