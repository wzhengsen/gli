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

#include "../type.hpp"

namespace gli{
namespace detail
{
	enum convertMode
	{
		CONVERT_MODE_CAST,
		CONVERT_MODE_NORM,
		CONVERT_MODE_SRGB
	};

	template <typename textureType, typename retType, typename T, precision P, template <typename, precision> class vecType, convertMode mode = CONVERT_MODE_CAST>
	struct convert
	{
		static vecType<retType, P> fetch(textureType const & Texture, typename textureType::dim_type const & TexelCoord, typename textureType::size_type Level)
		{
			return vecType<retType, P>(Texture.load<vecType<T, P> >(TexelCoord, Level));
		}

		static void write(textureType & Texture, typename textureType::dim_type const & TexelCoord, typename textureType::size_type Level, vecType<retType, P> const & Texel)
		{
			Texture.store<vecType<T, P> >(TexelCoord, Level, vecType<T, P>(Texel));
		}
	};

	template <typename textureType, typename retType, typename T, precision P, template <typename, precision> class vecType>
	struct convert<textureType, retType, T, P, vecType, CONVERT_MODE_NORM>
	{
		static vecType<retType, P> fetch(textureType const & Texture, typename textureType::dim_type const & TexelCoord, typename textureType::size_type Level)
		{
			vecType<T, P> const Load(Texture.load<vecType<T, P> >(TexelCoord, Level));
			return compNormalize<retType>(Load);
		}

		static void write(textureType & Texture, typename textureType::dim_type const & TexelCoord, typename textureType::size_type Level, vecType<retType, P> const & Texel)
		{
			vecType<T, P> const Store(compScale<T>(Texel));
			Texture.store<vecType<T, P> >(TexelCoord, Level, Store);
		}
	};

	template <typename textureType, typename retType, typename T, precision P, template <typename, precision> class vecType>
	struct convert<textureType, retType, T, P, vecType, CONVERT_MODE_SRGB>
	{
		static vecType<retType, P> fetch(textureType const & Texture, typename textureType::dim_type const & TexelCoord, typename textureType::size_type Level)
		{
			return convertSRGBToLinear(compNormalize<retType>(Texture.load<vecType<T, P> >(TexelCoord, Level)));
		}

		static void write(textureType & Texture, typename textureType::dim_type const & TexelCoord, typename textureType::size_type Level, vecType<retType, P> const & Texel)
		{
			Texture.store<vecType<T, P> >(TexelCoord, Level, gli::compScale<T>(convertLinearToSRGB(Texel)));
		}
	};
}//namespace detail
}//namespace gli
