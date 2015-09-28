//////////////////////////////////////////////////////////////////////////////////
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
/// @file gli/core/texture_2d.cpp
/// @date 2011-10-11 / 2013-11-25
/// @author Christophe Riccio
///////////////////////////////////////////////////////////////////////////////////

#include <gli/gli.hpp>
#include <glm/gtx/component_wise.hpp>
#include <glm/gtc/color_space.hpp>
#include <glm/gtc/epsilon.hpp>
#include <glm/gtc/packing.hpp>
#include <ctime>
#include <limits>

namespace gli{
namespace detail
{
	inline float passThrought(float const & texcoord)
	{
		return texcoord;
	}

	// Default
	template <typename floatType, glm::precision P>
	struct texelFetchDefault{
	static glm::tvec4<floatType, P> call(texture2D const & Texture, texture2D::dim_type const & TexelCoord, texture2D::size_type Level)
	{
		return glm::tvec4<floatType, P>(0.0f, 0.0f, 0.0f, 1.0f);
	}};

	template <typename floatType, glm::precision P, gli::format Format = FORMAT_UNDEFINED>
	struct texelFetch{
	static glm::tvec4<floatType, P> call(texture2D const & Texture, texture2D::dim_type const & TexelCoord, texture2D::size_type Level)
	{
		return glm::tvec4<floatType, P>(0.0f, 0.0f, 0.0f, 1.0f);
	}};

	// Packed
	template <typename floatType, glm::precision P>
	struct texelFetch<floatType, P, FORMAT_RGB10A2_UNORM>{
	static glm::tvec4<floatType, P> call(texture2D const & Texture, texture2D::dim_type const & TexelCoord, texture2D::size_type Level)
	{
		return glm::tvec4<floatType, P>(glm::unpackUnorm3x10_1x2(Texture.load<glm::uint32>(TexelCoord, Level)));
	}};

	template <typename floatType, glm::precision P>
	struct texelFetch<floatType, P, FORMAT_RGB10A2_SNORM> {
	static glm::tvec4<floatType, P> call(texture2D const & Texture, texture2D::dim_type const & TexelCoord, texture2D::size_type Level)
	{
		return glm::tvec4<floatType, P>(glm::unpackSnorm3x10_1x2(Texture.load<glm::uint32>(TexelCoord, Level)));
	}};

	template <typename floatType, glm::precision P>
	struct texelFetch<floatType, P, FORMAT_RGB10A2_USCALED> {
	static glm::tvec4<floatType, P> call(texture2D const & Texture, texture2D::dim_type const & TexelCoord, texture2D::size_type Level)
	{
		return glm::tvec4<floatType, P>(glm::unpackU3x10_1x2(Texture.load<glm::uint32>(TexelCoord, Level)));
	}};

	template <typename floatType, glm::precision P>
	struct texelFetch<floatType, P, FORMAT_RGB10A2_SSCALED> {
	static glm::tvec4<floatType, P> call(texture2D const & Texture, texture2D::dim_type const & TexelCoord, texture2D::size_type Level)
	{
		return glm::tvec4<floatType, P>(glm::unpackI3x10_1x2(Texture.load<glm::uint32>(TexelCoord, Level)));
	}};

	// Half
	template <typename floatType, glm::precision P>
	struct texelFetch<floatType, P, FORMAT_R16_SFLOAT>{
	static glm::tvec4<floatType, P> call(texture2D const & Texture, texture2D::dim_type const & TexelCoord, texture2D::size_type Level)
	{
		return glm::tvec4<floatType, P>(glm::unpackHalf1x16(Texture.load<glm::uint16>(TexelCoord, Level)), 0.0f, 0.0f, 1.0f);
	}};

	template <typename floatType, glm::precision P>
	struct texelFetch<floatType, P, FORMAT_RG16_SFLOAT>{
	static glm::tvec4<floatType, P> call(texture2D const & Texture, texture2D::dim_type const & TexelCoord, texture2D::size_type Level)
	{
		return glm::tvec4<floatType, P>(glm::unpackHalf2x16(Texture.load<glm::uint16>(TexelCoord, Level)), 0.0f, 1.0f);
	}};

	template <typename floatType, glm::precision P>
	struct texelFetch<floatType, P, FORMAT_RGB16_SFLOAT>{
	static glm::tvec4<floatType, P> call(texture2D const & Texture, texture2D::dim_type const & TexelCoord, texture2D::size_type Level)
	{
		glm::u16vec3 const Packed = Texture.load<glm::u16vec3>(TexelCoord, Level);
		return glm::tvec4<floatType, P>(glm::unpackHalf1x16(Packed.x), glm::unpackHalf1x16(Packed.y), glm::unpackHalf1x16(Packed.z), 1.0f);
	}};

	template <typename floatType, glm::precision P>
	struct texelFetch<floatType, P, FORMAT_RGBA16_SFLOAT>{
	static glm::tvec4<floatType, P> call(texture2D const & Texture, texture2D::dim_type const & TexelCoord, texture2D::size_type Level)
	{
		return glm::tvec4<floatType, P>(glm::unpackHalf4x16(Texture.load<glm::uint64>(TexelCoord, Level)));
	}};

	// Normalize
	template <typename floatType, glm::precision P, typename valType>
	struct texelFetchNorm1{
	static glm::tvec4<floatType, P> call(texture2D const & Texture, texture2D::dim_type const & TexelCoord, texture2D::size_type Level)
	{
		return glm::tvec4<floatType, P>(glm::compNormalize<floatType>(Texture.load<glm::tvec1<valType, P> >(TexelCoord, Level)).x, 0.0f, 0.0f, 1.0f);
	}};

	template <typename floatType, glm::precision P, typename valType>
	struct texelFetchNorm2{
	static glm::tvec4<floatType, P> call(texture2D const & Texture, texture2D::dim_type const & TexelCoord, texture2D::size_type Level)
	{
		return glm::tvec4<floatType, P>(glm::compNormalize<floatType>(Texture.load<glm::tvec2<valType, P> >(TexelCoord, Level)), 0.0f, 1.0f);
	}};

	template <typename floatType, glm::precision P, typename valType>
	struct texelFetchNorm3{
	static glm::tvec4<floatType, P> call(texture2D const & Texture, texture2D::dim_type const & TexelCoord, texture2D::size_type Level)
	{
		return glm::tvec4<floatType, P>(glm::compNormalize<floatType>(Texture.load<glm::tvec3<valType, P> >(TexelCoord, Level)), 1.0f);
	}};

	template <typename floatType, glm::precision P, typename valType>
	struct texelFetchNorm4{
	static glm::tvec4<floatType, P> call(texture2D const & Texture, texture2D::dim_type const & TexelCoord, texture2D::size_type Level)
	{
		return glm::compNormalize<floatType>(Texture.load<glm::tvec4<valType, P> >(TexelCoord, Level));
	}};

	// sRGB
	template <typename floatType, glm::precision P>
	struct texelFetchSRGB1{
	static glm::tvec4<floatType, P> call(texture2D const & Texture, texture2D::dim_type const & TexelCoord, texture2D::size_type Level)
	{
		return glm::tvec4<floatType, P>(glm::convertSRGBToLinear(glm::compNormalize<floatType>(Texture.load<glm::tvec1<glm::u8, P> >(TexelCoord, Level))).x, 0.0f, 0.0f, 1.0f);
	}};

	template <typename floatType, glm::precision P>
	struct texelFetchSRGB2{
	static glm::tvec4<floatType, P> call(texture2D const & Texture, texture2D::dim_type const & TexelCoord, texture2D::size_type Level)
	{
		return glm::tvec4<floatType, P>(glm::convertSRGBToLinear(glm::compNormalize<floatType>(Texture.load<glm::tvec2<glm::u8, P> >(TexelCoord, Level))), 0.0f, 1.0f);
	}};

	template <typename floatType, glm::precision P>
	struct texelFetchSRGB3{
	static glm::tvec4<floatType, P> call(texture2D const & Texture, texture2D::dim_type const & TexelCoord, texture2D::size_type Level)
	{
		return glm::tvec4<floatType, P>(glm::convertSRGBToLinear(glm::compNormalize<floatType>(Texture.load<glm::tvec3<glm::u8, P> >(TexelCoord, Level))), 1.0f);
	}};

	template <typename floatType, glm::precision P>
	struct texelFetchSRGB4{
	static glm::tvec4<floatType, P> call(texture2D const & Texture, texture2D::dim_type const & TexelCoord, texture2D::size_type Level)
	{
		return glm::convertSRGBToLinear(glm::compNormalize<floatType>(Texture.load<glm::tvec4<glm::u8, P> >(TexelCoord, Level)));
	}};

	// Cast
	template <typename floatType, glm::precision P, typename valType>
	struct texelFetchCast1{
	static glm::tvec4<floatType, P> call(texture2D const & Texture, texture2D::dim_type const & TexelCoord, texture2D::size_type Level)
	{
		return glm::tvec4<floatType, P>(static_cast<floatType>(Texture.load<glm::tvec1<valType, P> >(TexelCoord, Level).x), static_cast<floatType>(0), static_cast<floatType>(0), static_cast<floatType>(1));
	}};

	template <typename floatType, glm::precision P, typename valType>
	struct texelFetchCast2{
	static glm::tvec4<floatType, P> call(texture2D const & Texture, texture2D::dim_type const & TexelCoord, texture2D::size_type Level)
	{
		return glm::tvec4<floatType, P>(glm::tvec2<floatType, P>(Texture.load<glm::tvec2<valType, P> >(TexelCoord, Level)), static_cast<floatType>(0), static_cast<floatType>(1));
	}};

	template <typename floatType, glm::precision P, typename valType>
	struct texelFetchCast3{
	static glm::tvec4<floatType, P> call(texture2D const & Texture, texture2D::dim_type const & TexelCoord, texture2D::size_type Level)
	{
		return glm::tvec4<floatType, P>(glm::tvec3<floatType, P>(Texture.load<glm::tvec3<valType, P> >(TexelCoord, Level)), static_cast<floatType>(1));
	}};

	template <typename floatType, glm::precision P, typename valType>
	struct texelFetchCast4{
	static glm::tvec4<floatType, P> call(texture2D const & Texture, texture2D::dim_type const & TexelCoord, texture2D::size_type Level)
	{
		return glm::tvec4<floatType, P>(Texture.load<glm::tvec4<valType, P> >(TexelCoord, Level));
	}};

	// Read
	template <typename floatType, glm::precision P, typename valType>
	struct texelFetchRead1{
	static glm::tvec4<floatType, P> call(texture2D const & Texture, texture2D::dim_type const & TexelCoord, texture2D::size_type Level)
	{
		return glm::tvec4<floatType, P>(static_cast<floatType>(Texture.load<glm::tvec1<valType, P> >(TexelCoord, Level).x), static_cast<floatType>(0), static_cast<floatType>(0), static_cast<floatType>(1));
	}};

	template <typename floatType, glm::precision P, typename valType>
	struct texelFetchRead2{
	static glm::tvec4<floatType, P> call(texture2D const & Texture, texture2D::dim_type const & TexelCoord, texture2D::size_type Level)
	{
		return glm::tvec4<floatType, P>(glm::tvec2<floatType, P>(Texture.load<glm::tvec2<valType, P> >(TexelCoord, Level)), static_cast<floatType>(0), static_cast<floatType>(1));
	}};

	template <typename floatType, glm::precision P, typename valType>
	struct texelFetchRead3{
	static glm::tvec4<floatType, P> call(texture2D const & Texture, texture2D::dim_type const & TexelCoord, texture2D::size_type Level)
	{
		return glm::tvec4<floatType, P>(Texture.load<glm::tvec3<valType, P>>(TexelCoord, Level), static_cast<valType>(1));
	}};

	template <typename floatType, glm::precision P, typename valType>
	struct texelFetchRead4{
	static glm::tvec4<floatType, P> call(texture2D const & Texture, texture2D::dim_type const & TexelCoord, texture2D::size_type Level)
	{
		return glm::tvec4<floatType, P>(Texture.load<glm::tvec4<valType, P>>(TexelCoord, Level));
	}};




	template <typename floatType, glm::precision P>
	struct texelWriteSRGB4{
	static void call(texture2D const & Texture, texture2D::dim_type const & TexelCoord, texture2D::size_type Level, glm::tvec4<floatType, P> const & Texel)
	{
		Texture.store<glm::tvec4<glm::u8, P> >(TexelCoord, Level, glm::tvec4<glm::u8, P>(glm::compScale(glm::convertLinearToSRGB(Texel))));
	}};

	template <typename floatType, glm::precision P, typename valType>
	struct texelWriteRGBA4 {
	static void call(texture2D const & Texture, texture2D::dim_type const & TexelCoord, texture2D::size_type Level, glm::tvec4<floatType, P> const & Texel)
	{
		Texture.store<glm::tvec4<valType, P> >(TexelCoord, Level, glm::tvec4<valType, P>(glm::compScale(Texel)));
	}};

}//namespace detail

	enum wrap
	{
		WRAP_CLAMP_TO_EDGE, WRAP_FIRST = WRAP_CLAMP_TO_EDGE,
		WRAP_CLAMP_TO_BORDER,
		WRAP_REPEAT,
		WRAP_MIRROR_REPEAT,
		WRAP_MIRROR_CLAMP_TO_EDGE,
		WRAP_MIRROR_CLAMP_TO_BORDER, WRAP_LAST = WRAP_MIRROR_CLAMP_TO_BORDER
	};

	enum
	{
		WRAP_COUNT = WRAP_LAST - WRAP_FIRST + 1
	};

	class sampler
	{
		typedef float (*wrapFunc)(float const & texcoord);

	protected:
		wrapFunc getFunc(wrap WrapMode) const
		{
			static wrapFunc Table[] =
			{
				glm::clamp,
				detail::passThrought,
				glm::repeat,
				glm::mirrorRepeat,
				glm::mirrorClamp,
				glm::mirrorClamp
			};
			static_assert(sizeof(Table) / sizeof(Table[0]) == WRAP_COUNT, "Table needs to be updated");

			return Table[WrapMode];
		}

	public:
		sampler(wrap Wrap, filter Mip, filter Min)
			: WrapMode(Wrap)
			, WrapFunc(getFunc(Wrap))
			, Mip(Mip)
			, Min(Min)
		{}

	protected:
		wrap WrapMode;
		wrapFunc WrapFunc;
		filter Mip;
		filter Min;
	};

	template <typename floatType, glm::precision P = glm::defaultp>
	class sampler2D : public sampler
	{
		typedef glm::tvec4<floatType, P> (*texelFetchFunc)(texture2D const & Texture, texture2D::dim_type const & TexelCoord, texture2D::size_type Level);
		typedef void (*texelWriteFunc)(texture2D const & Texture, texture2D::dim_type const & TexelCoord, texture2D::size_type Level, glm::tvec4<floatType, P> const & Texel);

		texelFetchFunc GetTexelFetchFunc(format Format) const
		{
			static texelFetchFunc Table[] =
			{
				detail::texelFetch<floatType, P, FORMAT_RG4_UNORM>::call,				// FORMAT_RG4_UNORM
				detail::texelFetch<floatType, P, FORMAT_RG4_USCALED>::call,				// FORMAT_RG4_USCALED
				detail::texelFetch<floatType, P, FORMAT_RGBA4_UNORM>::call,				// FORMAT_RGBA4_UNORM
				detail::texelFetch<floatType, P, FORMAT_RGBA4_USCALED>::call,			// FORMAT_RGBA4_USCALED
				detail::texelFetch<floatType, P, FORMAT_R5G6B5_UNORM>::call,			// FORMAT_R5G6B5_UNORM
				detail::texelFetch<floatType, P, FORMAT_R5G6B5_USCALED>::call,			// FORMAT_R5G6B5_USCALED
				detail::texelFetch<floatType, P, FORMAT_RGB5A1_UNORM>::call,			// FORMAT_RGB5A1_UNORM
				detail::texelFetch<floatType, P, FORMAT_RGB5A1_USCALED>::call,			// FORMAT_RGB5A1_USCALED

				detail::texelFetchNorm1<floatType, P, glm::u8>::call,					// FORMAT_R8_UNORM
				detail::texelFetchNorm1<floatType, P, glm::i8>::call,					// FORMAT_R8_SNORM
				detail::texelFetchCast1<floatType, P, glm::u8>::call,					// FORMAT_R8_USCALED
				detail::texelFetchCast1<floatType, P, glm::i8>::call,					// FORMAT_R8_SSCALED
				detail::texelFetchDefault<floatType, P>::call,							// FORMAT_R8_UINT
				detail::texelFetchDefault<floatType, P>::call,							// FORMAT_R8_SINT
				detail::texelFetchSRGB1<floatType, P>::call,							// FORMAT_R8_SRGB

				detail::texelFetchNorm2<floatType, P, glm::u8>::call,					// FORMAT_RG8_UNORM
				detail::texelFetchNorm2<floatType, P, glm::i8>::call,					// FORMAT_RG8_SNORM
				detail::texelFetchCast2<floatType, P, glm::u8>::call,					// FORMAT_RG8_USCALED
				detail::texelFetchCast2<floatType, P, glm::i8>::call,					// FORMAT_RG8_SSCALED
				detail::texelFetchDefault<floatType, P>::call,							// FORMAT_RG8_UINT
				detail::texelFetchDefault<floatType, P>::call,							// FORMAT_RG8_SINT
				detail::texelFetchSRGB2<floatType, P>::call,							// FORMAT_RG8_SRGB

				detail::texelFetchNorm3<floatType, P, glm::u8>::call,					// FORMAT_RGB8_UNORM
				detail::texelFetchNorm3<floatType, P, glm::i8>::call,					// FORMAT_RGB8_SNORM
				detail::texelFetchCast3<floatType, P, glm::u8>::call,					// FORMAT_RGB8_USCALED
				detail::texelFetchCast3<floatType, P, glm::i8>::call,					// FORMAT_RGB8_SSCALED
				detail::texelFetchDefault<floatType, P>::call,							// FORMAT_RGB8_UINT
				detail::texelFetchDefault<floatType, P>::call,							// FORMAT_RGB8_SINT
				detail::texelFetchSRGB3<floatType, P>::call,							// FORMAT_RGB8_SRGB

				detail::texelFetchNorm4<floatType, P, glm::u8>::call,					// FORMAT_RGBA8_UNORM
				detail::texelFetchNorm4<floatType, P, glm::i8>::call,					// FORMAT_RGBA8_SNORM
				detail::texelFetchCast4<floatType, P, glm::u8>::call,					// FORMAT_RGBA8_USCALED
				detail::texelFetchCast4<floatType, P, glm::i8>::call,					// FORMAT_RGBA8_SSCALED
				detail::texelFetchDefault<floatType, P>::call,							// FORMAT_RGBA8_UINT
				detail::texelFetchDefault<floatType, P>::call,							// FORMAT_RGBA8_SINT
				detail::texelFetchSRGB4<floatType, P>::call,							// FORMAT_RGBA8_SRGB

				detail::texelFetch<floatType, P, FORMAT_RGB10A2_UNORM>::call,			// FORMAT_RGB10A2_UNORM
				detail::texelFetch<floatType, P, FORMAT_RGB10A2_SNORM>::call,			// FORMAT_RGB10A2_SNORM
				detail::texelFetch<floatType, P, FORMAT_RGB10A2_USCALED>::call,			// FORMAT_RGB10A2_USCALED
				detail::texelFetch<floatType, P, FORMAT_RGB10A2_SSCALED>::call,			// FORMAT_RGB10A2_SSCALED
				detail::texelFetch<floatType, P, FORMAT_RGB10A2_UINT>::call,			// FORMAT_RGB10A2_UINT
				detail::texelFetch<floatType, P, FORMAT_RGB10A2_SINT>::call,			// FORMAT_RGB10A2_SINT

				detail::texelFetchNorm1<floatType, P, glm::u16>::call,					// FORMAT_R16_UNORM
				detail::texelFetchNorm1<floatType, P, glm::i16>::call,					// FORMAT_R16_SNORM
				detail::texelFetchCast1<floatType, P, glm::u16>::call,					// FORMAT_R16_USCALED
				detail::texelFetchCast1<floatType, P, glm::i16>::call,					// FORMAT_R16_SSCALED
				detail::texelFetchDefault<floatType, P>::call,							// FORMAT_R16_UINT
				detail::texelFetchDefault<floatType, P>::call,							// FORMAT_R16_SINT
				detail::texelFetch<floatType, P, FORMAT_R16_SFLOAT>::call,				// FORMAT_R16_SFLOAT

				detail::texelFetchNorm2<floatType, P, glm::u16>::call,					// FORMAT_RG16_UNORM
				detail::texelFetchNorm2<floatType, P, glm::i16>::call,					// FORMAT_RG16_SNORM
				detail::texelFetchCast2<floatType, P, glm::u16>::call,					// FORMAT_RG16_USCALED
				detail::texelFetchCast2<floatType, P, glm::i16>::call,					// FORMAT_RG16_SSCALED
				detail::texelFetchDefault<floatType, P>::call,							// FORMAT_RG16_UINT
				detail::texelFetchDefault<floatType, P>::call,							// FORMAT_RG16_SINT
				detail::texelFetch<floatType, P, FORMAT_RG16_SFLOAT>::call,				// FORMAT_RG16_SFLOAT

				detail::texelFetchNorm3<floatType, P, glm::u16>::call,					// FORMAT_RGB16_UNORM
				detail::texelFetchNorm3<floatType, P, glm::i16>::call,					// FORMAT_RGB16_SNORM
				detail::texelFetchCast3<floatType, P, glm::u16>::call,					// FORMAT_RGB16_USCALED
				detail::texelFetchCast3<floatType, P, glm::i16>::call,					// FORMAT_RGB16_SSCALED
				detail::texelFetchDefault<floatType, P>::call,							// FORMAT_RGB16_UINT
				detail::texelFetchDefault<floatType, P>::call,							// FORMAT_RGB16_SINT
				detail::texelFetch<floatType, P, FORMAT_RGB16_SFLOAT>::call,			// FORMAT_RGB16_SFLOAT

				detail::texelFetchNorm4<floatType, P, glm::u16>::call,					// FORMAT_RGBA16_UNORM
				detail::texelFetchNorm4<floatType, P, glm::i16>::call,					// FORMAT_RGBA16_SNORM
				detail::texelFetchCast4<floatType, P, glm::u16>::call,					// FORMAT_RGBA16_USCALED
				detail::texelFetchCast4<floatType, P, glm::i16>::call,					// FORMAT_RGBA16_SSCALED
				detail::texelFetchDefault<floatType, P>::call,							// FORMAT_RGBA16_UINT
				detail::texelFetchDefault<floatType, P>::call,							// FORMAT_RGBA16_SINT
				detail::texelFetch<floatType, P, FORMAT_RGBA16_SFLOAT>::call,			// FORMAT_RGBA16_SFLOAT

				detail::texelFetchRead1<floatType, P, glm::u32>::call,					// FORMAT_R32_UINT
				detail::texelFetchRead1<floatType, P, glm::i32>::call,					// FORMAT_R32_SINT
				detail::texelFetchRead1<floatType, P, glm::f32>::call,					// FORMAT_R32_SFLOAT
				detail::texelFetchRead2<floatType, P, glm::u32>::call,					// FORMAT_RG32_UINT
				detail::texelFetchRead2<floatType, P, glm::i32>::call,					// FORMAT_RG32_SINT
				detail::texelFetchRead2<floatType, P, glm::f32>::call,					// FORMAT_RG32_SFLOAT
				detail::texelFetchRead3<floatType, P, glm::u32>::call,					// FORMAT_RGB32_UINT
				detail::texelFetchRead3<floatType, P, glm::i32>::call,					// FORMAT_RGB32_SINT
				detail::texelFetchRead3<floatType, P, glm::f32>::call,					// FORMAT_RGB32_SFLOAT
				detail::texelFetchRead4<floatType, P, glm::u32>::call,					// FORMAT_RGBA32_UINT
				detail::texelFetchRead4<floatType, P, glm::i32>::call,					// FORMAT_RGBA32_SINT
				detail::texelFetchRead4<floatType, P, glm::f32>::call,					// FORMAT_RGBA32_SFLOAT

				detail::texelFetchRead1<floatType, P, glm::f64>::call,					// FORMAT_R64_SFLOAT
				detail::texelFetchRead2<floatType, P, glm::f64>::call,					// FORMAT_RG64_SFLOAT
				detail::texelFetchRead3<floatType, P, glm::f64>::call,					// FORMAT_RGB64_SFLOAT
				detail::texelFetchRead4<floatType, P, glm::f64>::call,					// FORMAT_RGBA64_SFLOAT

				detail::texelFetchDefault<floatType, P>::call,							// FORMAT_RG11B10_UFLOAT
				detail::texelFetchDefault<floatType, P>::call,							// FORMAT_RGB9E5_UFLOAT

				detail::texelFetchDefault<floatType, P>::call,							// FORMAT_D16_UNORM
				detail::texelFetchDefault<floatType, P>::call,							// FORMAT_D24_UNORM
				detail::texelFetchDefault<floatType, P>::call,							// FORMAT_D32_SFLOAT
				detail::texelFetchDefault<floatType, P>::call,							// FORMAT_S8_UINT
				detail::texelFetchDefault<floatType, P>::call,							// FORMAT_D16_UNORM_S8_UINT
				detail::texelFetchDefault<floatType, P>::call,							// FORMAT_D24_UNORM_S8_UINT
				detail::texelFetchDefault<floatType, P>::call,							// FORMAT_D32_SFLOAT_S8_UINT

				detail::texelFetchDefault<floatType, P>::call,							// FORMAT_RGB_DXT1_UNORM
				detail::texelFetchDefault<floatType, P>::call,							// FORMAT_RGB_DXT1_SRGB
				detail::texelFetchDefault<floatType, P>::call,							// FORMAT_RGBA_DXT1_UNORM
				detail::texelFetchDefault<floatType, P>::call,							// FORMAT_RGBA_DXT1_SRGB
				detail::texelFetchDefault<floatType, P>::call,							// FORMAT_RGBA_DXT3_UNORM
				detail::texelFetchDefault<floatType, P>::call,							// FORMAT_RGBA_DXT3_SRGB
				detail::texelFetchDefault<floatType, P>::call,							// FORMAT_RGBA_DXT5_UNORM
				detail::texelFetchDefault<floatType, P>::call,							// FORMAT_RGBA_DXT5_SRGB
				detail::texelFetchDefault<floatType, P>::call,							// FORMAT_R_ATI1N_UNORM
				detail::texelFetchDefault<floatType, P>::call,							// FORMAT_R_ATI1N_SNORM
				detail::texelFetchDefault<floatType, P>::call,							// FORMAT_RG_ATI2N_UNORM
				detail::texelFetchDefault<floatType, P>::call,							// FORMAT_RG_ATI2N_SNORM
				detail::texelFetchDefault<floatType, P>::call,							// FORMAT_RGB_BP_UFLOAT
				detail::texelFetchDefault<floatType, P>::call,							// FORMAT_RGB_BP_SFLOAT
				detail::texelFetchDefault<floatType, P>::call,							// FORMAT_RGBA_BP_UNORM
				detail::texelFetchDefault<floatType, P>::call,							// FORMAT_RGBA_BP_SRGB

				detail::texelFetchDefault<floatType, P>::call,							// FORMAT_RGB_ETC2_UNORM
				detail::texelFetchDefault<floatType, P>::call,							// FORMAT_RGB_ETC2_SRGB
				detail::texelFetchDefault<floatType, P>::call,							// FORMAT_RGBA_ETC2_A1_UNORM
				detail::texelFetchDefault<floatType, P>::call,							// FORMAT_RGBA_ETC2_A1_SRGB
				detail::texelFetchDefault<floatType, P>::call,							// FORMAT_RGBA_ETC2_UNORM
				detail::texelFetchDefault<floatType, P>::call,							// FORMAT_RGBA_ETC2_SRGB
				detail::texelFetchDefault<floatType, P>::call,							// FORMAT_R_EAC_UNORM
				detail::texelFetchDefault<floatType, P>::call,							// FORMAT_R_EAC_SNORM
				detail::texelFetchDefault<floatType, P>::call,							// FORMAT_RG_EAC_UNORM
				detail::texelFetchDefault<floatType, P>::call,							// FORMAT_RG_EAC_SNORM
				detail::texelFetchDefault<floatType, P>::call,							// FORMAT_ASTC_4x4_UNORM
				detail::texelFetchDefault<floatType, P>::call,							// FORMAT_ASTC_4x4_SRGB
				detail::texelFetchDefault<floatType, P>::call,							// FORMAT_ASTC_5x4_UNORM
				detail::texelFetchDefault<floatType, P>::call,							// FORMAT_ASTC_5x4_SRGB
				detail::texelFetchDefault<floatType, P>::call,							// FORMAT_ASTC_5x5_UNORM
				detail::texelFetchDefault<floatType, P>::call,							// FORMAT_ASTC_5x5_SRGB
				detail::texelFetchDefault<floatType, P>::call,							// FORMAT_ASTC_6x5_UNORM
				detail::texelFetchDefault<floatType, P>::call,							// FORMAT_ASTC_6x5_SRGB
				detail::texelFetchDefault<floatType, P>::call,							// FORMAT_ASTC_6x6_UNORM
				detail::texelFetchDefault<floatType, P>::call,							// FORMAT_ASTC_6x6_SRGB
				detail::texelFetchDefault<floatType, P>::call,							// FORMAT_ASTC_8x5_UNORM
				detail::texelFetchDefault<floatType, P>::call,							// FORMAT_ASTC_8x5_SRGB
				detail::texelFetchDefault<floatType, P>::call,							// FORMAT_ASTC_8x6_UNORM
				detail::texelFetchDefault<floatType, P>::call,							// FORMAT_ASTC_8x6_SRGB
				detail::texelFetchDefault<floatType, P>::call,							// FORMAT_ASTC_8x8_UNORM
				detail::texelFetchDefault<floatType, P>::call,							// FORMAT_ASTC_8x8_SRGB
				detail::texelFetchDefault<floatType, P>::call,							// FORMAT_ASTC_10x5_UNORM
				detail::texelFetchDefault<floatType, P>::call,							// FORMAT_ASTC_10x5_SRGB
				detail::texelFetchDefault<floatType, P>::call,							// FORMAT_ASTC_10x6_UNORM
				detail::texelFetchDefault<floatType, P>::call,							// FORMAT_ASTC_10x6_SRGB
				detail::texelFetchDefault<floatType, P>::call,							// FORMAT_ASTC_10x8_UNORM
				detail::texelFetchDefault<floatType, P>::call,							// FORMAT_ASTC_10x8_SRGB
				detail::texelFetchDefault<floatType, P>::call,							// FORMAT_ASTC_10x10_UNORM
				detail::texelFetchDefault<floatType, P>::call,							// FORMAT_ASTC_10x10_SRGB
				detail::texelFetchDefault<floatType, P>::call,							// FORMAT_ASTC_12x10_UNORM
				detail::texelFetchDefault<floatType, P>::call,							// FORMAT_ASTC_12x10_SRGB
				detail::texelFetchDefault<floatType, P>::call,							// FORMAT_ASTC_12x12_UNORM
				detail::texelFetchDefault<floatType, P>::call,							// FORMAT_ASTC_12x12_SRGB

				detail::texelFetchDefault<floatType, P>::call,							// FORMAT_BGRA4_UNORM
				detail::texelFetchDefault<floatType, P>::call,							// FORMAT_BGRA4_USCALED
				detail::texelFetchDefault<floatType, P>::call,							// FORMAT_B5G6R5_UNORM
				detail::texelFetchDefault<floatType, P>::call,							// FORMAT_B5G6R5_USCALED
				detail::texelFetchDefault<floatType, P>::call,							// FORMAT_BGR5A1_UNORM
				detail::texelFetchDefault<floatType, P>::call,							// FORMAT_BGR5A1_USCALED

				detail::texelFetchNorm3<floatType, P, glm::u8>::call,					// FORMAT_BGR8_UNORM
				detail::texelFetchNorm3<floatType, P, glm::i8>::call,					// FORMAT_BGR8_SNORM
				detail::texelFetchCast3<floatType, P, glm::u8>::call,					// FORMAT_BGR8_USCALED
				detail::texelFetchCast3<floatType, P, glm::i8>::call,					// FORMAT_BGR8_SSCALED
				detail::texelFetchDefault<floatType, P>::call,							// FORMAT_BGR8_UINT
				detail::texelFetchDefault<floatType, P>::call,							// FORMAT_BGR8_SINT
				detail::texelFetchSRGB3<floatType, P>::call,							// FORMAT_BGR8_SRGB

				detail::texelFetchNorm4<floatType, P, glm::u8>::call,					// FORMAT_BGRA8_UNORM
				detail::texelFetchNorm4<floatType, P, glm::i8>::call,					// FORMAT_BGRA8_SNORM
				detail::texelFetchCast4<floatType, P, glm::u8>::call,					// FORMAT_BGRA8_USCALED
				detail::texelFetchCast4<floatType, P, glm::i8>::call,					// FORMAT_BGRA8_SSCALED
				detail::texelFetchDefault<floatType, P>::call,							// FORMAT_BGRA8_UINT
				detail::texelFetchDefault<floatType, P>::call,							// FORMAT_BGRA8_SINT
				detail::texelFetchSRGB4<floatType, P>::call,							// FORMAT_BGRA8_SRGB

				detail::texelFetch<floatType, P, FORMAT_BGR10A2_UNORM>::call,			// FORMAT_BGR10A2_UNORM
				detail::texelFetch<floatType, P, FORMAT_BGR10A2_SNORM>::call,			// FORMAT_BGR10A2_SNORM
				detail::texelFetch<floatType, P, FORMAT_BGR10A2_USCALED>::call,			// FORMAT_BGR10A2_USCALED
				detail::texelFetch<floatType, P, FORMAT_BGR10A2_SSCALED>::call,			// FORMAT_BGR10A2_SSCALED
				detail::texelFetch<floatType, P, FORMAT_BGR10A2_UINT>::call,			// FORMAT_BGR10A2_UINT
				detail::texelFetch<floatType, P, FORMAT_BGR10A2_SINT>::call,			// FORMAT_BGR10A2_SINT

				detail::texelFetchDefault<floatType, P>::call,							// FORMAT_RG3B2_UNORM
				detail::texelFetchNorm4<floatType, P, glm::u8>::call,					// FORMAT_BGRX8_UNORM
				detail::texelFetchSRGB4<floatType, P>::call,							// FORMAT_BGRX8_SRGB

				detail::texelFetchNorm1<floatType, P, glm::u8>::call,					// FORMAT_L8_UNORM
				detail::texelFetchNorm1<floatType, P, glm::u8>::call,					// FORMAT_A8_UNORM
				detail::texelFetchNorm2<floatType, P, glm::u8>::call,					// FORMAT_LA8_UNORM
				detail::texelFetchNorm1<floatType, P, glm::u16>::call,					// FORMAT_L16_UNORM
				detail::texelFetchNorm1<floatType, P, glm::u16>::call,					// FORMAT_A16_UNORM
				detail::texelFetchNorm2<floatType, P, glm::u16>::call,					// FORMAT_LA16_UNORM

				detail::texelFetchDefault<floatType, P>::call,							// FORMAT_RGB_PVRTC1_8X8_UNORM
				detail::texelFetchDefault<floatType, P>::call,							// FORMAT_RGB_PVRTC1_8X8_SRGB
				detail::texelFetchDefault<floatType, P>::call,							// FORMAT_RGB_PVRTC1_16X8_UNORM
				detail::texelFetchDefault<floatType, P>::call,							// FORMAT_RGB_PVRTC1_16X8_SRGB
				detail::texelFetchDefault<floatType, P>::call,							// FORMAT_RGBA_PVRTC1_8X8_UNORM
				detail::texelFetchDefault<floatType, P>::call,							// FORMAT_RGBA_PVRTC1_8X8_SRGB
				detail::texelFetchDefault<floatType, P>::call,							// FORMAT_RGBA_PVRTC1_16X8_UNORM
				detail::texelFetchDefault<floatType, P>::call,							// FORMAT_RGBA_PVRTC1_16X8_SRGB
				detail::texelFetchDefault<floatType, P>::call,							// FORMAT_RGBA_PVRTC2_4X4_UNORM
				detail::texelFetchDefault<floatType, P>::call,							// FORMAT_RGBA_PVRTC2_4X4_SRGB
				detail::texelFetchDefault<floatType, P>::call,							// FORMAT_RGBA_PVRTC2_8X4_UNORM
				detail::texelFetchDefault<floatType, P>::call,							// FORMAT_RGBA_PVRTC2_8X4_SRGB

				detail::texelFetchDefault<floatType, P>::call,							// FORMAT_RGB_ETC_UNORM
				detail::texelFetchDefault<floatType, P>::call,							// FORMAT_RGB_ATC_UNORM
				detail::texelFetchDefault<floatType, P>::call,							// FORMAT_RGBA_ATC_EXPLICIT_UNORM
				detail::texelFetchDefault<floatType, P>::call							// FORMAT_RGBA_ATC_INTERPOLATED_UNORM
			};
			static_assert(sizeof(Table) / sizeof(Table[0]) == FORMAT_COUNT, "Texel fetch functions need to be updated");

			return Table[Format - FORMAT_FIRST];
		};

	public:
		sampler2D(texture2D const & Texture, wrap Wrap, filter Mip, filter Min, glm::tvec4<floatType, P> const & BorderColor)
			: sampler(Wrap, Texture.levels() > 1 ? Mip : FILTER_NEAREST, Min)
			, Texture(Texture)
			, TexelFetch(this->GetTexelFetchFunc(this->Texture.format()))
			, BorderColor(BorderColor)
		{
			static_assert(std::numeric_limits<floatType>::is_iec559, "'sampler2D' accepts only floating-point types for 'floatType' template parameter");
			assert(!Texture.empty());
			assert(!is_compressed(Texture.format()));
		}

		glm::tvec4<floatType, P> texel_fetch(texture2D::dim_type const & TexelCoord, texture2D::size_type const & Level) const
		{
			return this->TexelFetch(this->Texture, TexelCoord, Level);
		}

		template <template <typename, glm::precision> class vecType, typename valType>
		void texel_write(texture2D::dim_type const & TexelCoord, texture2D::size_type const & Level, glm::tvec4<floatType, P> const & Texel)
		{
			this->TexelWrite(this->Texture, TexelCoord, Level, Texel);
		}

/*
		template <template <typename, glm::precision> class vecType, typename valType>
		glm::tvec4<floatType, P> texture_lod(texture2D::texcoord_type const & Texcoord, float Level) const
		{
			texture2D::texcoord_type const TexcoordWrap(this->WrapFunc(Texcoord.x), this->WrapFunc(Texcoord.y));

			texture2D::size_type const minLevel = texture2D::size_type(glm::floor(Level));
			texture2D::size_type const maxLevel = texture2D::size_type(glm::ceil(Level));

			if(this->Mip == FILTER_LINEAR)
			{
				glm::tvec4<floatType, P> const minTexel = this->Min == FILTER_LINEAR ? this->texture_lod_linear<vecType, valType>(TexcoordWrap, minLevel) : this->texture_lod_nearest<vecType, valType>(TexcoordWrap, minLevel);
				glm::tvec4<floatType, P> const maxTexel = this->Min == FILTER_LINEAR ? this->texture_lod_linear<vecType, valType>(TexcoordWrap, maxLevel) : this->texture_lod_nearest<vecType, valType>(TexcoordWrap, minLevel);
				return mix(minTexel, maxTexel, glm::fract(Level));
			}
			else
			{
				texture2D::size_type const level = texture2D::size_type(glm::round(Level));
				return this->Min == FILTER_LINEAR ? this->texture_lod_linear<vecType, valType>(TexcoordWrap, level) : this->texture_lod_nearest<vecType, valType>(TexcoordWrap, minLevel);
			}
		}

	private:
		template <template <typename, glm::precision> class vecType, typename valType>
		glm::tvec4<floatType, P> texture_lod_linear(texture2D::texcoord_type const & Texcoord, texture2D::size_type Level) const
		{
			texture2D::dim_type const TexelDim = this->Texture.dimensions(Level);
			bool const IsSRGB = gli::is_srgb(Texture.format());

			int const s_below = int(glm::floor(Texcoord.s * static_cast<floatType>(TexelDim.x - 1)));
			int const s_above = int(glm::ceil( Texcoord.s * static_cast<floatType>(TexelDim.x - 1)));
			int const t_below = int(glm::floor(Texcoord.t * static_cast<floatType>(TexelDim.y - 1)));
			int const t_above = int(glm::ceil( Texcoord.t * static_cast<floatType>(TexelDim.y - 1)));

			glm::bvec4 UseBorderColor(
				s_below < 0 || s_below > static_cast<int>(TexelDim.x),
				s_above < 0 || s_above > static_cast<int>(TexelDim.x),
				t_below < 0 || t_below > static_cast<int>(TexelDim.y),
				t_above < 0 || t_above > static_cast<int>(TexelDim.y));

			floatType const s_below_normalized = s_below / static_cast<floatType>(TexelDim.x);
			floatType const t_below_normalized = t_below / static_cast<floatType>(TexelDim.y);

			glm::tvec4<floatType, P> Texel00(this->BorderColor);
			glm::tvec4<floatType, P> Texel10(this->BorderColor);
			glm::tvec4<floatType, P> Texel11(this->BorderColor);
			glm::tvec4<floatType, P> Texel01(this->BorderColor);

			if(!UseBorderColor[0] && !UseBorderColor[2])
				Texel00 = this->texel_fetch<vecType, valType>(gli::dim2_t(s_below, t_below), Level);

			if(!UseBorderColor[1] && !UseBorderColor[2])
				Texel10 = this->texel_fetch<vecType, valType>(gli::dim2_t(s_above, t_below), Level);

			if(!UseBorderColor[1] && !UseBorderColor[3])
				Texel11 = this->texel_fetch<vecType, valType>(gli::dim2_t(s_above, t_above), Level);

			if(!UseBorderColor[0] && !UseBorderColor[3])
				Texel11 = this->texel_fetch<vecType, valType>(gli::dim2_t(s_below, t_above), Level);

			floatType const BlendA = static_cast<floatType>(Texcoord.s - s_below_normalized) * static_cast<floatType>(TexelDim.x - 1);
			floatType const BlendB = static_cast<floatType>(Texcoord.s - s_below_normalized) * static_cast<floatType>(TexelDim.x - 1);
			floatType const BlendC = static_cast<floatType>(Texcoord.t - t_below_normalized) * static_cast<floatType>(TexelDim.y - 1);

			glm::tvec4<floatType, P> const ValueA(glm::mix(Texel00, Texel10, BlendA));
			glm::tvec4<floatType, P> const ValueB(glm::mix(Texel01, Texel11, BlendB));

			return glm::mix(ValueA, ValueB, BlendC);
		}

		template <template <typename, glm::precision> class vecType, typename valType>
		glm::tvec4<floatType, P> texture_lod_nearest(texture2D::texcoord_type const & Texcoord, texture2D::size_type Level) const
		{
			texture2D::dim_type const TexelDim = this->Texture.dimensions(Level);

			int const s = int(glm::floor(Texcoord.s * static_cast<floatType>(TexelDim.x - 1)));
			int const t = int(glm::floor(Texcoord.t * static_cast<floatType>(TexelDim.y - 1)));

			bool const UseBorderColor = s > static_cast<int>(TexelDim.x) || s < 0 || t > static_cast<int>(TexelDim.y) || t < 0;

			return UseBorderColor ? this->BorderColor : glm::compNormalize<floatType>(this->Texture.load<vecType<valType, P> >(gli::dim2_t(s, t), Level));
		}
*/
	private:
		texture2D Texture;
		texelFetchFunc TexelFetch;
		glm::tvec4<floatType, P> BorderColor;
	};

	typedef sampler2D<float> fsampler2D;
	typedef sampler2D<double> dsampler2D;

}//namespace gli

namespace load
{
	int test()
	{
		int Error(0);

		gli::texture2D Texture(gli::FORMAT_RGBA8_UNORM, gli::texture2D::dim_type(4, 2), 1);
		*(Texture.data<glm::u8vec4>() + 0) = glm::u8vec4(255,   0,   0, 255);
		*(Texture.data<glm::u8vec4>() + 1) = glm::u8vec4(255, 128,   0, 255);
		*(Texture.data<glm::u8vec4>() + 2) = glm::u8vec4(255, 255,   0, 255);
		*(Texture.data<glm::u8vec4>() + 3) = glm::u8vec4(128, 255,   0, 255);
		*(Texture.data<glm::u8vec4>() + 4) = glm::u8vec4(  0, 255,   0, 255);
		*(Texture.data<glm::u8vec4>() + 5) = glm::u8vec4(  0, 255, 255, 255);
		*(Texture.data<glm::u8vec4>() + 6) = glm::u8vec4(  0,   0, 255, 255);
		*(Texture.data<glm::u8vec4>() + 7) = glm::u8vec4(255,   0, 255, 255);

		glm::u8vec4 Data0 = Texture.load<glm::u8vec4>(gli::texture2D::dim_type(0, 0), 0);
		glm::u8vec4 Data1 = Texture.load<glm::u8vec4>(gli::texture2D::dim_type(1, 0), 0);
		glm::u8vec4 Data2 = Texture.load<glm::u8vec4>(gli::texture2D::dim_type(2, 0), 0);
		glm::u8vec4 Data3 = Texture.load<glm::u8vec4>(gli::texture2D::dim_type(3, 0), 0);
		glm::u8vec4 Data4 = Texture.load<glm::u8vec4>(gli::texture2D::dim_type(0, 1), 0);
		glm::u8vec4 Data5 = Texture.load<glm::u8vec4>(gli::texture2D::dim_type(1, 1), 0);
		glm::u8vec4 Data6 = Texture.load<glm::u8vec4>(gli::texture2D::dim_type(2, 1), 0);
		glm::u8vec4 Data7 = Texture.load<glm::u8vec4>(gli::texture2D::dim_type(3, 1), 0);

		Error += Data0 == glm::u8vec4(255,   0,   0, 255) ? 0 : 1;
		Error += Data1 == glm::u8vec4(255, 128,   0, 255) ? 0 : 1;
		Error += Data2 == glm::u8vec4(255, 255,   0, 255) ? 0 : 1;
		Error += Data3 == glm::u8vec4(128, 255,   0, 255) ? 0 : 1;
		Error += Data4 == glm::u8vec4(  0, 255,   0, 255) ? 0 : 1;
		Error += Data5 == glm::u8vec4(  0, 255, 255, 255) ? 0 : 1;
		Error += Data6 == glm::u8vec4(  0,   0, 255, 255) ? 0 : 1;
		Error += Data7 == glm::u8vec4(255,   0, 255, 255) ? 0 : 1;

		return Error;
	}
}//namespace load

namespace fetch_rgb10a2_snorm
{
	int test()
	{
		int Error(0);

		glm::vec4 Colors[] =
		{
			glm::vec4(1.0f, 0.0f, 0.0f, 1.0f),
			glm::vec4(1.0f, 0.5f, 0.0f, 1.0f),
			glm::vec4(1.0f, 1.0f, 0.0f, 1.0f),
			glm::vec4(0.0f, 1.0f, 0.0f, 1.0f),
			glm::vec4(0.0f, 1.0f, 1.0f, 1.0f),
			glm::vec4(0.0f, 0.5f, 1.0f, 1.0f),
			glm::vec4(0.0f, 0.0f, 1.0f, 1.0f),
			glm::vec4(1.0f, 0.0f, 1.0f, 1.0f)
		};

		glm::uint32 Packed[8];
		for (std::size_t i = 0; i < 8; ++i)
			Packed[i] = glm::packSnorm3x10_1x2(Colors[i]);

		gli::texture2D Texture(gli::FORMAT_RGB10A2_SNORM, gli::texture2D::dim_type(4, 2), 1);
		for (std::size_t i = 0; i < 8; ++i)
			*(Texture.data<glm::uint32>() + i) = Packed[i];

		glm::uint32 Loaded[8];
		Loaded[0] = Texture.load<glm::uint32>(gli::texture2D::dim_type(0, 0), 0);
		Loaded[1] = Texture.load<glm::uint32>(gli::texture2D::dim_type(1, 0), 0);
		Loaded[2] = Texture.load<glm::uint32>(gli::texture2D::dim_type(2, 0), 0);
		Loaded[3] = Texture.load<glm::uint32>(gli::texture2D::dim_type(3, 0), 0);
		Loaded[4] = Texture.load<glm::uint32>(gli::texture2D::dim_type(0, 1), 0);
		Loaded[5] = Texture.load<glm::uint32>(gli::texture2D::dim_type(1, 1), 0);
		Loaded[6] = Texture.load<glm::uint32>(gli::texture2D::dim_type(2, 1), 0);
		Loaded[7] = Texture.load<glm::uint32>(gli::texture2D::dim_type(3, 1), 0);

		for (std::size_t i = 0; i < 8; ++i)
			Error += Packed[i] == Loaded[i] ? 0 : 1;

		glm::vec4 Unpacked[8];
		for (std::size_t i = 0; i < 8; ++i)
			Unpacked[i] = glm::unpackSnorm3x10_1x2(Loaded[i]);

		for (std::size_t i = 0; i < 8; ++i)
			Error += glm::all(glm::epsilonEqual(Unpacked[i], Colors[i], 0.01f)) ? 0 : 1;

		gli::fsampler2D Sampler(Texture, gli::WRAP_CLAMP_TO_EDGE, gli::FILTER_LINEAR, gli::FILTER_LINEAR, glm::vec4(0.0f, 0.5f, 1.0f, 1.0f));

		glm::vec4 Data0 = Sampler.texel_fetch(gli::texture2D::dim_type(0, 0), 0);
		glm::vec4 Data1 = Sampler.texel_fetch(gli::texture2D::dim_type(1, 0), 0);
		glm::vec4 Data2 = Sampler.texel_fetch(gli::texture2D::dim_type(2, 0), 0);
		glm::vec4 Data3 = Sampler.texel_fetch(gli::texture2D::dim_type(3, 0), 0);
		glm::vec4 Data4 = Sampler.texel_fetch(gli::texture2D::dim_type(0, 1), 0);
		glm::vec4 Data5 = Sampler.texel_fetch(gli::texture2D::dim_type(1, 1), 0);
		glm::vec4 Data6 = Sampler.texel_fetch(gli::texture2D::dim_type(2, 1), 0);
		glm::vec4 Data7 = Sampler.texel_fetch(gli::texture2D::dim_type(3, 1), 0);

		float const Epsilon = 1.f / 255.f * 0.5f;

		Error += glm::all(glm::epsilonEqual(Data0, Colors[0], Epsilon)) ? 0 : 1;
		Error += glm::all(glm::epsilonEqual(Data1, Colors[1], Epsilon)) ? 0 : 1;
		Error += glm::all(glm::epsilonEqual(Data2, Colors[2], Epsilon)) ? 0 : 1;
		Error += glm::all(glm::epsilonEqual(Data3, Colors[3], Epsilon)) ? 0 : 1;
		Error += glm::all(glm::epsilonEqual(Data4, Colors[4], Epsilon)) ? 0 : 1;
		Error += glm::all(glm::epsilonEqual(Data5, Colors[5], Epsilon)) ? 0 : 1;
		Error += glm::all(glm::epsilonEqual(Data6, Colors[6], Epsilon)) ? 0 : 1;
		Error += glm::all(glm::epsilonEqual(Data7, Colors[7], Epsilon)) ? 0 : 1;

		return Error;
	}
}//namespace fetch_rgb10a2_snorm

namespace fetch_rgb10a2_unorm
{
	int test()
	{
		int Error(0);

		glm::vec4 Colors[] =
		{
			glm::vec4(1.0f, 0.0f, 0.0f, 1.0f),
			glm::vec4(1.0f, 0.5f, 0.0f, 1.0f),
			glm::vec4(1.0f, 1.0f, 0.0f, 1.0f),
			glm::vec4(0.0f, 1.0f, 0.0f, 1.0f),
			glm::vec4(0.0f, 1.0f, 1.0f, 1.0f),
			glm::vec4(0.0f, 0.5f, 1.0f, 1.0f),
			glm::vec4(0.0f, 0.0f, 1.0f, 1.0f),
			glm::vec4(1.0f, 0.0f, 1.0f, 1.0f)
		};

		glm::uint32 Packed[8];
		for(std::size_t i = 0; i < 8; ++i)
			Packed[i] = glm::packUnorm3x10_1x2(Colors[i]);

		gli::texture2D Texture(gli::FORMAT_RGB10A2_UNORM, gli::texture2D::dim_type(4, 2), 1);
		for(std::size_t i = 0; i < 8; ++i)
			*(Texture.data<glm::uint32>() + i) = Packed[i];

		glm::uint32 Loaded[8];
		Loaded[0] = Texture.load<glm::uint32>(gli::texture2D::dim_type(0, 0), 0);
		Loaded[1] = Texture.load<glm::uint32>(gli::texture2D::dim_type(1, 0), 0);
		Loaded[2] = Texture.load<glm::uint32>(gli::texture2D::dim_type(2, 0), 0);
		Loaded[3] = Texture.load<glm::uint32>(gli::texture2D::dim_type(3, 0), 0);
		Loaded[4] = Texture.load<glm::uint32>(gli::texture2D::dim_type(0, 1), 0);
		Loaded[5] = Texture.load<glm::uint32>(gli::texture2D::dim_type(1, 1), 0);
		Loaded[6] = Texture.load<glm::uint32>(gli::texture2D::dim_type(2, 1), 0);
		Loaded[7] = Texture.load<glm::uint32>(gli::texture2D::dim_type(3, 1), 0);

		for(std::size_t i = 0; i < 8; ++i)
			Error += Packed[i] == Loaded[i] ? 0 : 1;

		glm::vec4 Unpacked[8];
		for(std::size_t i = 0; i < 8; ++i)
			Unpacked[i] = glm::unpackUnorm3x10_1x2(Loaded[i]);

		for (std::size_t i = 0; i < 8; ++i)
			Error += glm::all(glm::epsilonEqual(Unpacked[i], Colors[i], 0.01f)) ? 0 : 1;

		gli::fsampler2D Sampler(Texture, gli::WRAP_CLAMP_TO_EDGE, gli::FILTER_LINEAR, gli::FILTER_LINEAR, glm::vec4(0.0f, 0.5f, 1.0f, 1.0f));

		glm::vec4 Data0 = Sampler.texel_fetch(gli::texture2D::dim_type(0, 0), 0);
		glm::vec4 Data1 = Sampler.texel_fetch(gli::texture2D::dim_type(1, 0), 0);
		glm::vec4 Data2 = Sampler.texel_fetch(gli::texture2D::dim_type(2, 0), 0);
		glm::vec4 Data3 = Sampler.texel_fetch(gli::texture2D::dim_type(3, 0), 0);
		glm::vec4 Data4 = Sampler.texel_fetch(gli::texture2D::dim_type(0, 1), 0);
		glm::vec4 Data5 = Sampler.texel_fetch(gli::texture2D::dim_type(1, 1), 0);
		glm::vec4 Data6 = Sampler.texel_fetch(gli::texture2D::dim_type(2, 1), 0);
		glm::vec4 Data7 = Sampler.texel_fetch(gli::texture2D::dim_type(3, 1), 0);

		float const Epsilon = 1.f / 255.f * 0.5f;

		Error += glm::all(glm::epsilonEqual(Data0, Colors[0], Epsilon)) ? 0 : 1;
		Error += glm::all(glm::epsilonEqual(Data1, Colors[1], Epsilon)) ? 0 : 1;
		Error += glm::all(glm::epsilonEqual(Data2, Colors[2], Epsilon)) ? 0 : 1;
		Error += glm::all(glm::epsilonEqual(Data3, Colors[3], Epsilon)) ? 0 : 1;
		Error += glm::all(glm::epsilonEqual(Data4, Colors[4], Epsilon)) ? 0 : 1;
		Error += glm::all(glm::epsilonEqual(Data5, Colors[5], Epsilon)) ? 0 : 1;
		Error += glm::all(glm::epsilonEqual(Data6, Colors[6], Epsilon)) ? 0 : 1;
		Error += glm::all(glm::epsilonEqual(Data7, Colors[7], Epsilon)) ? 0 : 1;

		return Error;
	}
}//namespace fetch_rgb10a2_unorm

namespace fetch_rgba8_unorm
{
	int test()
	{
		int Error(0);

		glm::vec4 Colors[] =
		{
			glm::vec4(1.0f, 0.0f, 0.0f, 1.0f),
			glm::vec4(1.0f, 0.5f, 0.0f, 1.0f),
			glm::vec4(1.0f, 1.0f, 0.0f, 1.0f),
			glm::vec4(0.0f, 1.0f, 0.0f, 1.0f),
			glm::vec4(0.0f, 1.0f, 1.0f, 1.0f),
			glm::vec4(0.0f, 0.5f, 1.0f, 1.0f),
			glm::vec4(0.0f, 0.0f, 1.0f, 1.0f),
			glm::vec4(1.0f, 0.0f, 1.0f, 1.0f)
		};

		gli::texture2D Texture(gli::FORMAT_RGBA8_UNORM, gli::texture2D::dim_type(4, 2), 1);
		*(Texture.data<glm::u8vec4>() + 0) = glm::u8vec4(Colors[0] * 255.f);
		*(Texture.data<glm::u8vec4>() + 1) = glm::u8vec4(Colors[1] * 255.f);
		*(Texture.data<glm::u8vec4>() + 2) = glm::u8vec4(Colors[2] * 255.f);
		*(Texture.data<glm::u8vec4>() + 3) = glm::u8vec4(Colors[3] * 255.f);
		*(Texture.data<glm::u8vec4>() + 4) = glm::u8vec4(Colors[4] * 255.f);
		*(Texture.data<glm::u8vec4>() + 5) = glm::u8vec4(Colors[5] * 255.f);
		*(Texture.data<glm::u8vec4>() + 6) = glm::u8vec4(Colors[6] * 255.f);
		*(Texture.data<glm::u8vec4>() + 7) = glm::u8vec4(Colors[7] * 255.f);

		gli::fsampler2D Sampler(Texture, gli::WRAP_CLAMP_TO_EDGE, gli::FILTER_LINEAR, gli::FILTER_LINEAR, glm::vec4(0.0f, 0.5f, 1.0f, 1.0f));

		glm::vec4 Data0 = Sampler.texel_fetch(gli::texture2D::dim_type(0, 0), 0);
		glm::vec4 Data1 = Sampler.texel_fetch(gli::texture2D::dim_type(1, 0), 0);
		glm::vec4 Data2 = Sampler.texel_fetch(gli::texture2D::dim_type(2, 0), 0);
		glm::vec4 Data3 = Sampler.texel_fetch(gli::texture2D::dim_type(3, 0), 0);
		glm::vec4 Data4 = Sampler.texel_fetch(gli::texture2D::dim_type(0, 1), 0);
		glm::vec4 Data5 = Sampler.texel_fetch(gli::texture2D::dim_type(1, 1), 0);
		glm::vec4 Data6 = Sampler.texel_fetch(gli::texture2D::dim_type(2, 1), 0);
		glm::vec4 Data7 = Sampler.texel_fetch(gli::texture2D::dim_type(3, 1), 0);

		float const Epsilon = 1.f / 255.f * 0.5f;

		Error += glm::all(glm::epsilonEqual(Data0, Colors[0], Epsilon)) ? 0 : 1;
		Error += glm::all(glm::epsilonEqual(Data1, Colors[1], Epsilon)) ? 0 : 1;
		Error += glm::all(glm::epsilonEqual(Data2, Colors[2], Epsilon)) ? 0 : 1;
		Error += glm::all(glm::epsilonEqual(Data3, Colors[3], Epsilon)) ? 0 : 1;
		Error += glm::all(glm::epsilonEqual(Data4, Colors[4], Epsilon)) ? 0 : 1;
		Error += glm::all(glm::epsilonEqual(Data5, Colors[5], Epsilon)) ? 0 : 1;
		Error += glm::all(glm::epsilonEqual(Data6, Colors[6], Epsilon)) ? 0 : 1;
		Error += glm::all(glm::epsilonEqual(Data7, Colors[7], Epsilon)) ? 0 : 1;

		return Error;
	}
}//namespace fetch_rgba8_unorm

namespace fetch_rgba8_srgb
{
	int test()
	{
		int Error(0);

		glm::vec4 Colors[] =
		{
			glm::vec4(1.0f, 0.0f, 0.0f, 1.0f),
			glm::vec4(1.0f, 0.5f, 0.0f, 1.0f),
			glm::vec4(1.0f, 1.0f, 0.0f, 1.0f),
			glm::vec4(0.0f, 1.0f, 0.0f, 1.0f),
			glm::vec4(0.0f, 1.0f, 1.0f, 1.0f),
			glm::vec4(0.0f, 0.5f, 1.0f, 1.0f),
			glm::vec4(0.0f, 0.0f, 1.0f, 1.0f),
			glm::vec4(1.0f, 0.0f, 1.0f, 1.0f)
		};

		glm::u8vec4 StoreSRGB00(glm::convertLinearToSRGB(Colors[0]) * 255.f);
		glm::u8vec4 StoreSRGB10(glm::convertLinearToSRGB(Colors[1]) * 255.f);
		glm::u8vec4 StoreSRGB20(glm::convertLinearToSRGB(Colors[2]) * 255.f);
		glm::u8vec4 StoreSRGB30(glm::convertLinearToSRGB(Colors[3]) * 255.f);
		glm::u8vec4 StoreSRGB01(glm::convertLinearToSRGB(Colors[4]) * 255.f);
		glm::u8vec4 StoreSRGB11(glm::convertLinearToSRGB(Colors[5]) * 255.f);
		glm::u8vec4 StoreSRGB21(glm::convertLinearToSRGB(Colors[6]) * 255.f);
		glm::u8vec4 StoreSRGB31(glm::convertLinearToSRGB(Colors[7]) * 255.f);

		gli::texture2D Texture(gli::FORMAT_RGBA8_SRGB, gli::texture2D::dim_type(4, 2), 1);
		Texture.store(gli::dim2_t(0, 0), 0, StoreSRGB00);
		Texture.store(gli::dim2_t(1, 0), 0, StoreSRGB10);
		Texture.store(gli::dim2_t(2, 0), 0, StoreSRGB20);
		Texture.store(gli::dim2_t(3, 0), 0, StoreSRGB30);
		Texture.store(gli::dim2_t(0, 1), 0, StoreSRGB01);
		Texture.store(gli::dim2_t(1, 1), 0, StoreSRGB11);
		Texture.store(gli::dim2_t(2, 1), 0, StoreSRGB21);
		Texture.store(gli::dim2_t(3, 1), 0, StoreSRGB31);

		{
			gli::fsampler2D Sampler(Texture, gli::WRAP_CLAMP_TO_EDGE, gli::FILTER_LINEAR, gli::FILTER_LINEAR, glm::vec4(0.0f, 0.5f, 1.0f, 1.0f));

			glm::vec4 Data0 = Sampler.texel_fetch(gli::texture2D::dim_type(0, 0), 0);
			glm::vec4 Data1 = Sampler.texel_fetch(gli::texture2D::dim_type(1, 0), 0);
			glm::vec4 Data2 = Sampler.texel_fetch(gli::texture2D::dim_type(2, 0), 0);
			glm::vec4 Data3 = Sampler.texel_fetch(gli::texture2D::dim_type(3, 0), 0);
			glm::vec4 Data4 = Sampler.texel_fetch(gli::texture2D::dim_type(0, 1), 0);
			glm::vec4 Data5 = Sampler.texel_fetch(gli::texture2D::dim_type(1, 1), 0);
			glm::vec4 Data6 = Sampler.texel_fetch(gli::texture2D::dim_type(2, 1), 0);
			glm::vec4 Data7 = Sampler.texel_fetch(gli::texture2D::dim_type(3, 1), 0);

			Error += glm::all(glm::epsilonEqual(Data0, Colors[0], 0.01f)) ? 0 : 1;
			Error += glm::all(glm::epsilonEqual(Data1, Colors[1], 0.01f)) ? 0 : 1;
			Error += glm::all(glm::epsilonEqual(Data2, Colors[2], 0.01f)) ? 0 : 1;
			Error += glm::all(glm::epsilonEqual(Data3, Colors[3], 0.01f)) ? 0 : 1;
			Error += glm::all(glm::epsilonEqual(Data4, Colors[4], 0.01f)) ? 0 : 1;
			Error += glm::all(glm::epsilonEqual(Data5, Colors[5], 0.01f)) ? 0 : 1;
			Error += glm::all(glm::epsilonEqual(Data6, Colors[6], 0.01f)) ? 0 : 1;
			Error += glm::all(glm::epsilonEqual(Data7, Colors[7], 0.01f)) ? 0 : 1;
		}

		{
			gli::dsampler2D Sampler(Texture, gli::WRAP_CLAMP_TO_EDGE, gli::FILTER_LINEAR, gli::FILTER_LINEAR, glm::dvec4(0.0f, 0.5f, 1.0f, 1.0f));

			glm::dvec4 Data0 = Sampler.texel_fetch(gli::texture2D::dim_type(0, 0), 0);
			glm::dvec4 Data1 = Sampler.texel_fetch(gli::texture2D::dim_type(1, 0), 0);
			glm::dvec4 Data2 = Sampler.texel_fetch(gli::texture2D::dim_type(2, 0), 0);
			glm::dvec4 Data3 = Sampler.texel_fetch(gli::texture2D::dim_type(3, 0), 0);
			glm::dvec4 Data4 = Sampler.texel_fetch(gli::texture2D::dim_type(0, 1), 0);
			glm::dvec4 Data5 = Sampler.texel_fetch(gli::texture2D::dim_type(1, 1), 0);
			glm::dvec4 Data6 = Sampler.texel_fetch(gli::texture2D::dim_type(2, 1), 0);
			glm::dvec4 Data7 = Sampler.texel_fetch(gli::texture2D::dim_type(3, 1), 0);

			Error += glm::all(glm::epsilonEqual(Data0, glm::dvec4(Colors[0]), 0.01)) ? 0 : 1;
			Error += glm::all(glm::epsilonEqual(Data1, glm::dvec4(Colors[1]), 0.01)) ? 0 : 1;
			Error += glm::all(glm::epsilonEqual(Data2, glm::dvec4(Colors[2]), 0.01)) ? 0 : 1;
			Error += glm::all(glm::epsilonEqual(Data3, glm::dvec4(Colors[3]), 0.01)) ? 0 : 1;
			Error += glm::all(glm::epsilonEqual(Data4, glm::dvec4(Colors[4]), 0.01)) ? 0 : 1;
			Error += glm::all(glm::epsilonEqual(Data5, glm::dvec4(Colors[5]), 0.01)) ? 0 : 1;
			Error += glm::all(glm::epsilonEqual(Data6, glm::dvec4(Colors[6]), 0.01)) ? 0 : 1;
			Error += glm::all(glm::epsilonEqual(Data7, glm::dvec4(Colors[7]), 0.01)) ? 0 : 1;
		}

		return Error;
	}
}//namespace fetch_rgba8_srgb

/*
namespace sampler
{
	int test()
	{
		int Error(0);

		gli::texture2D::dim_type const Size(512);
		gli::texture2D TextureA(gli::FORMAT_RGBA8_UNORM, Size, 1);

		{
			std::clock_t TimeStart = std::clock();

			for(std::size_t y = 0; y < Size.y; ++y)
			for(std::size_t x = 0; x < Size.x; ++x)
			{
				gli::texel_write(TextureA, gli::texture2D::dim_type(x, y), 0, glm::u8vec4(255, 128,   0, 255));
			}

			std::clock_t TimeEnd = std::clock();

			printf("texel_write(texture2D) - Time: %lu\n", TimeEnd - TimeStart);
		}

		gli::texture2D TextureB(gli::FORMAT_RGBA8_UNORM, Size, 1);

		{
			gli::sampler2D<float> Sampler(TextureB, gli::WRAP_CLAMP_TO_EDGE, gli::FILTER_LINEAR, gli::FILTER_LINEAR, glm::vec4(0.0f, 0.5f, 1.0f, 1.0f));

			{
				std::clock_t TimeStart = std::clock();

				for(std::size_t y = 0; y < Size.y; ++y)
				for(std::size_t x = 0; x < Size.x; ++x)
				{
					Sampler.texel_write<glm::tvec4, glm::u8>(gli::texture2D::dim_type(x, y), 0, glm::vec4(1.0f, 0.5f, 0.0f, 1.0f));
				}

				std::clock_t TimeEnd = std::clock();

				printf("sampler2D::texel_write - Time: %lu\n", TimeEnd - TimeStart);
			}

			{
				std::clock_t TimeStart = std::clock();

				for(float y = -0.5f; y < 1.5f; y += 0.025f)
				for(float x = -0.5f; x < 1.5f; x += 0.025f)
				{
					glm::vec4 ColorA = Sampler.texture_lod<glm::tvec4, glm::u8>(gli::texture2D::texcoord_type(x, y), 0.0f);
					Error += glm::all(glm::equal(ColorA, glm::vec4(1.0f, 0.5f, 0.0f, 1.0f))) ? 0 : 1;
				}

				std::clock_t TimeEnd = std::clock();

				printf("sampler2D::texture_lod - Time: %lu\n", TimeEnd - TimeStart);
			}
		}

		Error += TextureA == TextureB ? 0 : 1;

		return Error;
	}
}//namespace sampler

namespace clamp_to_border
{
	int test()
	{
		int Error(0);

		glm::vec4 const Orange(1.0f, 0.5f, 0.0f, 1.0f);
		glm::vec4 const Blue(0.0f, 0.5f, 1.0f, 1.0f);

		gli::texture2D::dim_type const Size(32);
		gli::texture2D Texture(gli::FORMAT_RGBA8_UNORM, Size, 1);
		Texture.clear(Orange);

		gli::sampler2D<float> Sampler(Texture, gli::WRAP_CLAMP_TO_BORDER, gli::FILTER_LINEAR, gli::FILTER_LINEAR, Blue);

		{
			glm::vec4 const Texel = Sampler.texture_lod<glm::tvec4, glm::u8>(gli::vec2(0.5f, 0.5f), 0.0f);
			Error += glm::all(glm::equal(Texel, Orange)) ? 0 : 1;
		}
		{
			glm::vec4 const Texel = Sampler.texture_lod<glm::tvec4, glm::u8>(gli::vec2(-0.5f, -0.5f), 0.0f);
			Error += glm::all(glm::equal(Texel, Blue)) ? 0 : 1;
		}
		{
			glm::vec4 const Texel = Sampler.texture_lod<glm::tvec4, glm::u8>(gli::vec2(1.5f,-0.5f), 0.0f);
			Error += glm::all(glm::equal(Texel, Blue)) ? 0 : 1;
		}
		{
			glm::vec4 const Texel = Sampler.texture_lod<glm::tvec4, glm::u8>(gli::vec2(1.5f, 1.5f), 0.0f);
			Error += glm::all(glm::equal(Texel, Blue)) ? 0 : 1;
		}

		return Error;
	}
}//namespace clamp_to_border
*/
int main()
{
	int Error(0);

	Error += load::test();
	Error += fetch_rgba8_unorm::test();
	Error += fetch_rgba8_srgb::test();
	Error += fetch_rgb10a2_unorm::test();
	Error += fetch_rgb10a2_snorm::test();
	//Error += sampler::test();
	//Error += clamp_to_border::test();

	return Error;
}

