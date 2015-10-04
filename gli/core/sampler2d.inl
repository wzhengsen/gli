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
/// @file gli/core/sampler2d.inl
/// @date 2015-09-29 / 2015-09-29
/// @author Christophe Riccio
///////////////////////////////////////////////////////////////////////////////////

#include <glm/gtc/packing.hpp>
#include <glm/gtc/color_space.hpp>

namespace gli{
namespace detail
{
	// Default
	template <typename floatType, glm::precision P>
	struct texelFetchDefault {
		static glm::tvec4<floatType, P> call(texture2D const & Texture, texture2D::dim_type const & TexelCoord, texture2D::size_type Level)
		{
			return glm::tvec4<floatType, P>(0.0f, 0.0f, 0.0f, 1.0f);
		}
	};

	template <typename floatType, glm::precision P, gli::format Format = FORMAT_UNDEFINED>
	struct texelFetch {
		static glm::tvec4<floatType, P> call(texture2D const & Texture, texture2D::dim_type const & TexelCoord, texture2D::size_type Level)
		{
			return glm::tvec4<floatType, P>(0.0f, 0.0f, 0.0f, 1.0f);
		}
	};

	// Packed
	template <typename floatType, glm::precision P>
	struct texelFetch<floatType, P, FORMAT_RGB10A2_UNORM> {
		static glm::tvec4<floatType, P> call(texture2D const & Texture, texture2D::dim_type const & TexelCoord, texture2D::size_type Level)
		{
			return glm::tvec4<floatType, P>(glm::unpackUnorm3x10_1x2(Texture.load<glm::uint32>(TexelCoord, Level)));
		}
	};

	template <typename floatType, glm::precision P>
	struct texelFetch<floatType, P, FORMAT_RGB10A2_SNORM> {
		static glm::tvec4<floatType, P> call(texture2D const & Texture, texture2D::dim_type const & TexelCoord, texture2D::size_type Level)
		{
			return glm::tvec4<floatType, P>(glm::unpackSnorm3x10_1x2(Texture.load<glm::uint32>(TexelCoord, Level)));
		}
	};

	template <typename floatType, glm::precision P>
	struct texelFetch<floatType, P, FORMAT_RGB10A2_USCALED> {
		static glm::tvec4<floatType, P> call(texture2D const & Texture, texture2D::dim_type const & TexelCoord, texture2D::size_type Level)
		{
			return glm::tvec4<floatType, P>(glm::unpackU3x10_1x2(Texture.load<glm::uint32>(TexelCoord, Level)));
		}
	};

	template <typename floatType, glm::precision P>
	struct texelFetch<floatType, P, FORMAT_RGB10A2_SSCALED> {
		static glm::tvec4<floatType, P> call(texture2D const & Texture, texture2D::dim_type const & TexelCoord, texture2D::size_type Level)
		{
			return glm::tvec4<floatType, P>(glm::unpackI3x10_1x2(Texture.load<glm::uint32>(TexelCoord, Level)));
		}
	};

	// Half
	template <typename floatType, glm::precision P>
	struct texelFetch<floatType, P, FORMAT_R16_SFLOAT> {
		static glm::tvec4<floatType, P> call(texture2D const & Texture, texture2D::dim_type const & TexelCoord, texture2D::size_type Level)
		{
			return glm::tvec4<floatType, P>(glm::unpackHalf1x16(Texture.load<glm::uint16>(TexelCoord, Level)), 0.0f, 0.0f, 1.0f);
		}
	};

	template <typename floatType, glm::precision P>
	struct texelFetch<floatType, P, FORMAT_RG16_SFLOAT> {
		static glm::tvec4<floatType, P> call(texture2D const & Texture, texture2D::dim_type const & TexelCoord, texture2D::size_type Level)
		{
			return glm::tvec4<floatType, P>(glm::unpackHalf2x16(Texture.load<glm::uint16>(TexelCoord, Level)), 0.0f, 1.0f);
		}
	};

	template <typename floatType, glm::precision P>
	struct texelFetch<floatType, P, FORMAT_RGB16_SFLOAT> {
		static glm::tvec4<floatType, P> call(texture2D const & Texture, texture2D::dim_type const & TexelCoord, texture2D::size_type Level)
		{
			glm::u16vec3 const Packed = Texture.load<glm::u16vec3>(TexelCoord, Level);
			return glm::tvec4<floatType, P>(glm::unpackHalf1x16(Packed.x), glm::unpackHalf1x16(Packed.y), glm::unpackHalf1x16(Packed.z), 1.0f);
		}
	};

	template <typename floatType, glm::precision P>
	struct texelFetch<floatType, P, FORMAT_RGBA16_SFLOAT> {
		static glm::tvec4<floatType, P> call(texture2D const & Texture, texture2D::dim_type const & TexelCoord, texture2D::size_type Level)
		{
			return glm::tvec4<floatType, P>(glm::unpackHalf4x16(Texture.load<glm::uint64>(TexelCoord, Level)));
		}
	};

	// Normalize
	template <typename T, glm::precision P, typename valType, bool isFloat = false>
	struct texelFetchNorm1 {
		static glm::tvec4<T, P> call(texture2D const & Texture, texture2D::dim_type const & TexelCoord, texture2D::size_type Level)
		{
			return glm::tvec4<T, P>(0, 0, 0, 1);
		}
	};

	template <typename T, glm::precision P, typename valType>
	struct texelFetchNorm1<T, P, valType, true> {
		static glm::tvec4<T, P> call(texture2D const & Texture, texture2D::dim_type const & TexelCoord, texture2D::size_type Level)
		{
			return glm::tvec4<T, P>(glm::compNormalize<T>(Texture.load<glm::tvec1<valType, P> >(TexelCoord, Level)).x, 0.0f, 0.0f, 1.0f);
		}
	};

	template <typename T, glm::precision P, typename valType, bool isFloat = false>
	struct texelFetchNorm2 {
		static glm::tvec4<T, P> call(texture2D const & Texture, texture2D::dim_type const & TexelCoord, texture2D::size_type Level)
		{
			return glm::tvec4<T, P>(0, 0, 0, 1);
		}
	};

	template <typename T, glm::precision P, typename valType>
	struct texelFetchNorm2<T, P, valType, true> {
		static glm::tvec4<T, P> call(texture2D const & Texture, texture2D::dim_type const & TexelCoord, texture2D::size_type Level)
		{
			return glm::tvec4<T, P>(glm::compNormalize<T>(Texture.load<glm::tvec2<valType, P> >(TexelCoord, Level)), 0.0f, 1.0f);
		}
	};

	template <typename T, glm::precision P, typename valType, bool isFloat = false>
	struct texelFetchNorm3 {
		static glm::tvec4<T, P> call(texture2D const & Texture, texture2D::dim_type const & TexelCoord, texture2D::size_type Level)
		{
			return glm::tvec4<T, P>(0, 0, 0, 1);
		}
	};

	template <typename T, glm::precision P, typename valType>
	struct texelFetchNorm3<T, P, valType, true> {
		static glm::tvec4<T, P> call(texture2D const & Texture, texture2D::dim_type const & TexelCoord, texture2D::size_type Level)
		{
			return glm::tvec4<T, P>(glm::compNormalize<T>(Texture.load<glm::tvec3<valType, P> >(TexelCoord, Level)), 1.0f);
		}
	};

	template <typename T, glm::precision P, typename valType, bool isFloat = false>
	struct texelNorm4
	{
		static glm::tvec4<T, P> fetch(texture2D const & Texture, texture2D::dim_type const & TexelCoord, texture2D::size_type Level)
		{
			return glm::tvec4<T, P>(0, 0, 0, 1);
		}

		static void write(texture2D & Texture, texture2D::dim_type const & TexelCoord, texture2D::size_type Level, glm::tvec4<T, P> const & Texel)
		{}
	};

	template <typename T, glm::precision P, typename valType>
	struct texelNorm4<T, P, valType, true>
	{
		static glm::tvec4<T, P> fetch(texture2D const & Texture, texture2D::dim_type const & TexelCoord, texture2D::size_type Level)
		{
			return glm::compNormalize<T>(Texture.load<glm::tvec4<valType, P> >(TexelCoord, Level));
		}

		static void write(texture2D & Texture, texture2D::dim_type const & TexelCoord, texture2D::size_type Level, glm::tvec4<T, P> const & Texel)
		{
			Texture.store<glm::tvec4<valType, P> >(TexelCoord, Level, glm::compScale<valType>(Texel));
		}
	};


	// sRGB
	template <typename T, glm::precision P, bool isFloatSampler = false>
	struct texelFetchSRGB1 {
		static glm::tvec4<T, P> call(texture2D const & Texture, texture2D::dim_type const & TexelCoord, texture2D::size_type Level)
		{
			return glm::tvec4<T, P>(0, 0, 0, 1);
		}
	};

	template <typename T, glm::precision P>
	struct texelFetchSRGB1<T, P, true> {
		static glm::tvec4<T, P> call(texture2D const & Texture, texture2D::dim_type const & TexelCoord, texture2D::size_type Level)
		{
			return glm::tvec4<T, P>(glm::convertSRGBToLinear(glm::compNormalize<T>(Texture.load<glm::tvec1<glm::u8, P> >(TexelCoord, Level))).x, 0.0f, 0.0f, 1.0f);
		}
	};

	template <typename T, glm::precision P, bool isFloatSampler = false>
	struct texelFetchSRGB2 {
		static glm::tvec4<T, P> call(texture2D const & Texture, texture2D::dim_type const & TexelCoord, texture2D::size_type Level)
		{
			return glm::tvec4<T, P>(0, 0, 0, 1);
		}
	};

	template <typename T, glm::precision P>
	struct texelFetchSRGB2<T, P, true> {
		static glm::tvec4<T, P> call(texture2D const & Texture, texture2D::dim_type const & TexelCoord, texture2D::size_type Level)
		{
			return glm::tvec4<T, P>(glm::convertSRGBToLinear(glm::compNormalize<T>(Texture.load<glm::tvec2<glm::u8, P> >(TexelCoord, Level))), 0.0f, 1.0f);
		}
	};

	template <typename T, glm::precision P, bool isFloatSampler = false>
	struct texelFetchSRGB3 {
		static glm::tvec4<T, P> call(texture2D const & Texture, texture2D::dim_type const & TexelCoord, texture2D::size_type Level)
		{
			return glm::tvec4<T, P>(0, 0, 0, 1);
		}
	};

	template <typename T, glm::precision P>
	struct texelFetchSRGB3<T, P, true> {
		static glm::tvec4<T, P> call(texture2D const & Texture, texture2D::dim_type const & TexelCoord, texture2D::size_type Level)
		{
			return glm::tvec4<T, P>(glm::convertSRGBToLinear(glm::compNormalize<T>(Texture.load<glm::tvec3<glm::u8, P> >(TexelCoord, Level))), 1.0f);
		}
	};

	template <typename T, glm::precision P, bool isFloatSampler = false>
	struct texelFetchSRGB4 {
		static glm::tvec4<T, P> call(texture2D const & Texture, texture2D::dim_type const & TexelCoord, texture2D::size_type Level)
		{
			return glm::tvec4<T, P>(0, 0, 0, 1);
		}
	};

	template <typename T, glm::precision P>
	struct texelFetchSRGB4<T, P, true> {
		static glm::tvec4<T, P> call(texture2D const & Texture, texture2D::dim_type const & TexelCoord, texture2D::size_type Level)
		{
			return glm::convertSRGBToLinear(glm::compNormalize<T>(Texture.load<glm::tvec4<glm::u8, P> >(TexelCoord, Level)));
		}
	};

	// Cast
	template <typename floatType, glm::precision P, typename valType>
	struct texelFetchCast1 {
		static glm::tvec4<floatType, P> call(texture2D const & Texture, texture2D::dim_type const & TexelCoord, texture2D::size_type Level)
		{
			return glm::tvec4<floatType, P>(static_cast<floatType>(Texture.load<glm::tvec1<valType, P> >(TexelCoord, Level).x), static_cast<floatType>(0), static_cast<floatType>(0), static_cast<floatType>(1));
		}
	};

	template <typename floatType, glm::precision P, typename valType>
	struct texelFetchCast2 {
		static glm::tvec4<floatType, P> call(texture2D const & Texture, texture2D::dim_type const & TexelCoord, texture2D::size_type Level)
		{
			return glm::tvec4<floatType, P>(glm::tvec2<floatType, P>(Texture.load<glm::tvec2<valType, P> >(TexelCoord, Level)), static_cast<floatType>(0), static_cast<floatType>(1));
		}
	};

	template <typename floatType, glm::precision P, typename valType>
	struct texelFetchCast3 {
		static glm::tvec4<floatType, P> call(texture2D const & Texture, texture2D::dim_type const & TexelCoord, texture2D::size_type Level)
		{
			return glm::tvec4<floatType, P>(glm::tvec3<floatType, P>(Texture.load<glm::tvec3<valType, P> >(TexelCoord, Level)), static_cast<floatType>(1));
		}
	};

	template <typename floatType, glm::precision P, typename valType>
	struct texelFetchCast4 {
		static glm::tvec4<floatType, P> call(texture2D const & Texture, texture2D::dim_type const & TexelCoord, texture2D::size_type Level)
		{
			return glm::tvec4<floatType, P>(Texture.load<glm::tvec4<valType, P> >(TexelCoord, Level));
		}
	};

	// Read
	template <typename floatType, glm::precision P, typename valType>
	struct texelFetchRead1 {
		static glm::tvec4<floatType, P> call(texture2D const & Texture, texture2D::dim_type const & TexelCoord, texture2D::size_type Level)
		{
			return glm::tvec4<floatType, P>(static_cast<floatType>(Texture.load<glm::tvec1<valType, P> >(TexelCoord, Level).x), static_cast<floatType>(0), static_cast<floatType>(0), static_cast<floatType>(1));
		}
	};

	template <typename floatType, glm::precision P, typename valType>
	struct texelFetchRead2 {
		static glm::tvec4<floatType, P> call(texture2D const & Texture, texture2D::dim_type const & TexelCoord, texture2D::size_type Level)
		{
			return glm::tvec4<floatType, P>(glm::tvec2<floatType, P>(Texture.load<glm::tvec2<valType, P> >(TexelCoord, Level)), static_cast<floatType>(0), static_cast<floatType>(1));
		}
	};

	template <typename floatType, glm::precision P, typename valType>
	struct texelFetchRead3 {
		static glm::tvec4<floatType, P> call(texture2D const & Texture, texture2D::dim_type const & TexelCoord, texture2D::size_type Level)
		{
			return glm::tvec4<floatType, P>(Texture.load<glm::tvec3<valType, P>>(TexelCoord, Level), static_cast<valType>(1));
		}
	};

	template <typename floatType, glm::precision P, typename valType>
	struct texelFetchRead4 {
		static glm::tvec4<floatType, P> call(texture2D const & Texture, texture2D::dim_type const & TexelCoord, texture2D::size_type Level)
		{
			return glm::tvec4<floatType, P>(Texture.load<glm::tvec4<valType, P>>(TexelCoord, Level));
		}
	};

	template <typename floatType, glm::precision P>
	struct texelWriteSRGB4 {
		static void call(texture2D const & Texture, texture2D::dim_type const & TexelCoord, texture2D::size_type Level, glm::tvec4<floatType, P> const & Texel)
		{
			Texture.store<glm::tvec4<glm::u8, P> >(TexelCoord, Level, glm::tvec4<glm::u8, P>(glm::compScale(glm::convertLinearToSRGB(Texel))));
		}
	};

	template <typename floatType, glm::precision P, typename valType>
	struct texelWriteRGBA4 {
		static void call(texture2D const & Texture, texture2D::dim_type const & TexelCoord, texture2D::size_type Level, glm::tvec4<floatType, P> const & Texel)
		{
			Texture.store<glm::tvec4<valType, P> >(TexelCoord, Level, glm::tvec4<valType, P>(glm::compScale(Texel)));
		}
	};
}//namespace detail

	template <typename T, glm::precision P>
	inline sampler2D<T, P>::sampler2D(texture2D const & Texture, wrap Wrap, filter Mip, filter Min, glm::tvec4<T, P> const & BorderColor)
		: sampler(Wrap, Texture.levels() > 1 ? Mip : FILTER_NEAREST, Min)
		, Texture(Texture)
		, TexelFunc(this->get_texel_func(this->Texture.format()))
		, BorderColor(BorderColor)
	{
		assert(!Texture.empty());
		assert(!is_compressed(Texture.format()));
	}

	template <typename T, glm::precision P>
	inline glm::tvec4<T, P> sampler2D<T, P>::texel_fetch(texture2D::dim_type const & TexelCoord, texture2D::size_type const & Level) const
	{
		return this->TexelFunc.Fetch(this->Texture, TexelCoord, Level);
	}

	template <typename T, glm::precision P>
	inline void sampler2D<T, P>::texel_write(texture2D::dim_type const & TexelCoord, texture2D::size_type const & Level, glm::tvec4<T, P> const & Texel)
	{
		this->TexelFunc.Write(this->Texture, TexelCoord, Level, Texel);
	}

	template <typename T, glm::precision P>
	inline glm::tvec4<T, P> sampler2D<T, P>::texture_lod(texture2D::samplecoord_type const & Texcoord, float Level) const
	{
		assert(std::numeric_limits<T>::is_iec559);

		texture2D::samplecoord_type const TexcoordWrap(this->WrapFunc(Texcoord.x), this->WrapFunc(Texcoord.y));

		texture2D::size_type const minLevel = texture2D::size_type(glm::floor(Level));
		texture2D::size_type const maxLevel = texture2D::size_type(glm::ceil(Level));

		if (this->Mip == FILTER_LINEAR)
		{
			glm::tvec4<T, P> const minTexel = this->Min == FILTER_LINEAR ? this->texture_lod_linear(TexcoordWrap, minLevel) : this->texture_lod_nearest(TexcoordWrap, minLevel);
			glm::tvec4<T, P> const maxTexel = this->Min == FILTER_LINEAR ? this->texture_lod_linear(TexcoordWrap, maxLevel) : this->texture_lod_nearest(TexcoordWrap, minLevel);
			return mix(minTexel, maxTexel, glm::fract(Level));
		} else
		{
			texture2D::size_type const level = texture2D::size_type(glm::round(Level));
			return this->Min == FILTER_LINEAR ? this->texture_lod_linear(TexcoordWrap, level) : this->texture_lod_nearest(TexcoordWrap, minLevel);
		}
	}

	template <typename T, glm::precision P>
	inline glm::tvec4<T, P> sampler2D<T, P>::texture_lod_linear(texture2D::samplecoord_type const & Texcoord, texture2D::size_type Level) const
	{
		assert(std::numeric_limits<T>::is_iec559);

		texture2D::dim_type const TexelDim = this->Texture.dimensions(Level);

		int const s_below = int(glm::floor(Texcoord.s * static_cast<T>(TexelDim.x - 1)));
		int const s_above = int(glm::ceil(Texcoord.s * static_cast<T>(TexelDim.x - 1)));
		int const t_below = int(glm::floor(Texcoord.t * static_cast<T>(TexelDim.y - 1)));
		int const t_above = int(glm::ceil(Texcoord.t * static_cast<T>(TexelDim.y - 1)));

		glm::bvec4 UseBorderColor(
			s_below < 0 || s_below > static_cast<int>(TexelDim.x),
			s_above < 0 || s_above > static_cast<int>(TexelDim.x),
			t_below < 0 || t_below > static_cast<int>(TexelDim.y),
			t_above < 0 || t_above > static_cast<int>(TexelDim.y));

		T const s_below_normalized = s_below / static_cast<T>(TexelDim.x);
		T const t_below_normalized = t_below / static_cast<T>(TexelDim.y);

		glm::tvec4<T, P> Texel00(this->BorderColor);
		glm::tvec4<T, P> Texel10(this->BorderColor);
		glm::tvec4<T, P> Texel11(this->BorderColor);
		glm::tvec4<T, P> Texel01(this->BorderColor);

		if (!UseBorderColor[0] && !UseBorderColor[2])
			Texel00 = this->texel_fetch(gli::dim2_t(s_below, t_below), Level);

		if (!UseBorderColor[1] && !UseBorderColor[2])
			Texel10 = this->texel_fetch(gli::dim2_t(s_above, t_below), Level);

		if (!UseBorderColor[1] && !UseBorderColor[3])
			Texel11 = this->texel_fetch(gli::dim2_t(s_above, t_above), Level);

		if (!UseBorderColor[0] && !UseBorderColor[3])
			Texel01 = this->texel_fetch(gli::dim2_t(s_below, t_above), Level);

		T const BlendA = static_cast<T>(Texcoord.s - s_below_normalized) * static_cast<T>(TexelDim.x - 1);
		T const BlendB = static_cast<T>(Texcoord.s - s_below_normalized) * static_cast<T>(TexelDim.x - 1);
		T const BlendC = static_cast<T>(Texcoord.t - t_below_normalized) * static_cast<T>(TexelDim.y - 1);

		glm::tvec4<T, P> const ValueA(glm::mix(Texel00, Texel10, BlendA));
		glm::tvec4<T, P> const ValueB(glm::mix(Texel01, Texel11, BlendB));
		glm::tvec4<T, P> const Texel(glm::mix(ValueA, ValueB, BlendC));
		return Texel;
	}

	template <typename T, glm::precision P>
	inline glm::tvec4<T, P> sampler2D<T, P>::texture_lod_nearest(texture2D::samplecoord_type const & Texcoord, texture2D::size_type Level) const
	{
		assert(std::numeric_limits<T>::is_iec559);

		texture2D::dim_type const TexelDim = this->Texture.dimensions(Level);

		int const s = int(glm::floor(Texcoord.s * static_cast<T>(TexelDim.x - 1)));
		int const t = int(glm::floor(Texcoord.t * static_cast<T>(TexelDim.y - 1)));

		bool const UseBorderColor = s > static_cast<int>(TexelDim.x) || s < 0 || t > static_cast<int>(TexelDim.y) || t < 0;

		return UseBorderColor ? this->BorderColor : this->TexelFunc.Fetch(this->Texture, gli::dim2_t(s, t), Level);
	}

	template <typename T, glm::precision P>
	inline void sampler2D<T, P>::generate_mipmaps()
	{
		assert(!this->Texture.empty());
		assert(!is_compressed(this->Texture.format()));

		this->generate_mipmaps(this->Texture.base_level(), this->Texture.max_level());
	}

	template <typename T, glm::precision P>
	inline void sampler2D<T, P>::generate_mipmaps(typename sampler2D<T, P>::size_type BaseLevel, typename sampler2D<T, P>::size_type MaxLevel)
	{
		assert(!this->Texture.empty());
		assert(!is_compressed(this->Texture.format()));
		assert(this->Texture.max_level() >= MaxLevel);
		assert(BaseLevel <= MaxLevel);

		texture2D Result(this->Texture.format(), this->Texture.dimensions(BaseLevel), MaxLevel - BaseLevel + 1);

		for(texture2D::size_type Level = BaseLevel; Level < MaxLevel; ++Level)
		{
			texture2D::dim_type const DimDst = Result.dimensions(Level + 1);
			samplecoord_type const SampleDimDst(DimDst);

			for(std::size_t j = 0; j < DimDst.y; ++j)
			for(std::size_t i = 0; i < DimDst.x; ++i)
			{
				std::size_t const x = (i << 1);
				std::size_t const y = (j << 1);

				glm::tvec4<T, P> Texel00 = this->texture_lod(samplecoord_type(x + 0, y + 0) / SampleDimDst, static_cast<float>(Level));
				glm::tvec4<T, P> Texel01 = this->texture_lod(samplecoord_type(x + 0, y + 1) / SampleDimDst, static_cast<float>(Level));
				glm::tvec4<T, P> Texel11 = this->texture_lod(samplecoord_type(x + 1, y + 1) / SampleDimDst, static_cast<float>(Level));
				glm::tvec4<T, P> Texel10 = this->texture_lod(samplecoord_type(x + 1, y + 0) / SampleDimDst, static_cast<float>(Level));

				glm::tvec4<T, P> const Texel = (Texel00 + Texel01 + Texel11 + Texel10) * static_cast<float>(0.25);
				Result.store(texture2D::dim_type(i, j), Level + 1, Texel);
			}
		}

		this->Texture = Result;
	}

	template <typename T, glm::precision P>
	inline typename sampler2D<T, P>::texelFunc sampler2D<T, P>::get_texel_func(format Format) const
	{
		static texelFunc Table[] =
		{
			{detail::texelFetch<T, P, FORMAT_RG4_UNORM>::call, nullptr},				// FORMAT_RG4_UNORM
			{detail::texelFetch<T, P, FORMAT_RG4_USCALED>::call, nullptr},				// FORMAT_RG4_USCALED
			{detail::texelFetch<T, P, FORMAT_RGBA4_UNORM>::call, nullptr},				// FORMAT_RGBA4_UNORM
			{detail::texelFetch<T, P, FORMAT_RGBA4_USCALED>::call, nullptr},			// FORMAT_RGBA4_USCALED
			{detail::texelFetch<T, P, FORMAT_R5G6B5_UNORM>::call, nullptr},			// FORMAT_R5G6B5_UNORM
			{detail::texelFetch<T, P, FORMAT_R5G6B5_USCALED>::call, nullptr},			// FORMAT_R5G6B5_USCALED
			{detail::texelFetch<T, P, FORMAT_RGB5A1_UNORM>::call, nullptr},			// FORMAT_RGB5A1_UNORM
			{detail::texelFetch<T, P, FORMAT_RGB5A1_USCALED>::call, nullptr},			// FORMAT_RGB5A1_USCALED

			{detail::texelFetchNorm1<T, P, glm::u8, std::numeric_limits<T>::is_iec559>::call, nullptr},			// FORMAT_R8_UNORM
			{detail::texelFetchNorm1<T, P, glm::i8, std::numeric_limits<T>::is_iec559>::call, nullptr},			// FORMAT_R8_SNORM
			{detail::texelFetchCast1<T, P, glm::u8>::call, nullptr},					// FORMAT_R8_USCALED
			{detail::texelFetchCast1<T, P, glm::i8>::call, nullptr},					// FORMAT_R8_SSCALED
			{detail::texelFetchDefault<T, P>::call, nullptr},							// FORMAT_R8_UINT
			{detail::texelFetchDefault<T, P>::call, nullptr},							// FORMAT_R8_SINT
			{detail::texelFetchSRGB1<T, P, std::numeric_limits<T>::is_iec559>::call, nullptr},							// FORMAT_R8_SRGB

			{detail::texelFetchNorm2<T, P, glm::u8, std::numeric_limits<T>::is_iec559>::call, nullptr},			// FORMAT_RG8_UNORM
			{detail::texelFetchNorm2<T, P, glm::i8, std::numeric_limits<T>::is_iec559>::call, nullptr},			// FORMAT_RG8_SNORM
			{detail::texelFetchCast2<T, P, glm::u8>::call, nullptr},					// FORMAT_RG8_USCALED
			{detail::texelFetchCast2<T, P, glm::i8>::call, nullptr},					// FORMAT_RG8_SSCALED
			{detail::texelFetchDefault<T, P>::call, nullptr},							// FORMAT_RG8_UINT
			{detail::texelFetchDefault<T, P>::call, nullptr},							// FORMAT_RG8_SINT
			{detail::texelFetchSRGB2<T, P, std::numeric_limits<T>::is_iec559>::call, nullptr},							// FORMAT_RG8_SRGB

			{detail::texelFetchNorm3<T, P, glm::u8, std::numeric_limits<T>::is_iec559>::call, nullptr},			// FORMAT_RGB8_UNORM
			{detail::texelFetchNorm3<T, P, glm::i8, std::numeric_limits<T>::is_iec559>::call, nullptr},			// FORMAT_RGB8_SNORM
			{detail::texelFetchCast3<T, P, glm::u8>::call, nullptr},					// FORMAT_RGB8_USCALED
			{detail::texelFetchCast3<T, P, glm::i8>::call, nullptr},					// FORMAT_RGB8_SSCALED
			{detail::texelFetchDefault<T, P>::call, nullptr},							// FORMAT_RGB8_UINT
			{detail::texelFetchDefault<T, P>::call, nullptr},							// FORMAT_RGB8_SINT
			{detail::texelFetchSRGB3<T, P, std::numeric_limits<T>::is_iec559>::call, nullptr},							// FORMAT_RGB8_SRGB

			{detail::texelNorm4<T, P, glm::u8, std::numeric_limits<T>::is_iec559>::fetch, detail::texelNorm4<T, P, glm::u8, std::numeric_limits<T>::is_iec559>::write},	// FORMAT_RGBA8_UNORM
			{detail::texelNorm4<T, P, glm::i8, std::numeric_limits<T>::is_iec559>::fetch, detail::texelNorm4<T, P, glm::i8, std::numeric_limits<T>::is_iec559>::write},	// FORMAT_RGBA8_SNORM
			{detail::texelFetchCast4<T, P, glm::u8>::call, nullptr},										// FORMAT_RGBA8_USCALED
			{detail::texelFetchCast4<T, P, glm::i8>::call, nullptr},										// FORMAT_RGBA8_SSCALED
			{detail::texelFetchCast4<T, P, glm::u8>::call, nullptr},										// FORMAT_RGBA8_UINT
			{detail::texelFetchCast4<T, P, glm::i8>::call, nullptr},										// FORMAT_RGBA8_SINT
			{detail::texelFetchSRGB4<T, P, std::numeric_limits<T>::is_iec559>::call, nullptr},				// FORMAT_RGBA8_SRGB

			{detail::texelFetch<T, P, FORMAT_RGB10A2_UNORM>::call, nullptr},			// FORMAT_RGB10A2_UNORM
			{detail::texelFetch<T, P, FORMAT_RGB10A2_SNORM>::call, nullptr},			// FORMAT_RGB10A2_SNORM
			{detail::texelFetch<T, P, FORMAT_RGB10A2_USCALED>::call, nullptr},			// FORMAT_RGB10A2_USCALED
			{detail::texelFetch<T, P, FORMAT_RGB10A2_SSCALED>::call, nullptr},			// FORMAT_RGB10A2_SSCALED
			{detail::texelFetch<T, P, FORMAT_RGB10A2_UINT>::call, nullptr},			// FORMAT_RGB10A2_UINT
			{detail::texelFetch<T, P, FORMAT_RGB10A2_SINT>::call, nullptr},			// FORMAT_RGB10A2_SINT

			{detail::texelFetchNorm1<T, P, glm::u16, std::numeric_limits<T>::is_iec559>::call, nullptr},			// FORMAT_R16_UNORM
			{detail::texelFetchNorm1<T, P, glm::i16, std::numeric_limits<T>::is_iec559>::call, nullptr},			// FORMAT_R16_SNORM
			{detail::texelFetchCast1<T, P, glm::u16>::call, nullptr},					// FORMAT_R16_USCALED
			{detail::texelFetchCast1<T, P, glm::i16>::call, nullptr},					// FORMAT_R16_SSCALED
			{detail::texelFetchDefault<T, P>::call, nullptr},							// FORMAT_R16_UINT
			{detail::texelFetchDefault<T, P>::call, nullptr},							// FORMAT_R16_SINT
			{detail::texelFetch<T, P, FORMAT_R16_SFLOAT>::call, nullptr},				// FORMAT_R16_SFLOAT

			{detail::texelFetchNorm2<T, P, glm::u16, std::numeric_limits<T>::is_iec559>::call, nullptr},			// FORMAT_RG16_UNORM
			{detail::texelFetchNorm2<T, P, glm::i16, std::numeric_limits<T>::is_iec559>::call, nullptr},			// FORMAT_RG16_SNORM
			{detail::texelFetchCast2<T, P, glm::u16>::call, nullptr},					// FORMAT_RG16_USCALED
			{detail::texelFetchCast2<T, P, glm::i16>::call, nullptr},					// FORMAT_RG16_SSCALED
			{detail::texelFetchDefault<T, P>::call, nullptr},							// FORMAT_RG16_UINT
			{detail::texelFetchDefault<T, P>::call, nullptr},							// FORMAT_RG16_SINT
			{detail::texelFetch<T, P, FORMAT_RG16_SFLOAT>::call, nullptr},				// FORMAT_RG16_SFLOAT

			{detail::texelFetchNorm3<T, P, glm::u16, std::numeric_limits<T>::is_iec559>::call, nullptr},			// FORMAT_RGB16_UNORM
			{detail::texelFetchNorm3<T, P, glm::i16, std::numeric_limits<T>::is_iec559>::call, nullptr},			// FORMAT_RGB16_SNORM
			{detail::texelFetchCast3<T, P, glm::u16>::call, nullptr},					// FORMAT_RGB16_USCALED
			{detail::texelFetchCast3<T, P, glm::i16>::call, nullptr},					// FORMAT_RGB16_SSCALED
			{detail::texelFetchDefault<T, P>::call, nullptr},							// FORMAT_RGB16_UINT
			{detail::texelFetchDefault<T, P>::call, nullptr},							// FORMAT_RGB16_SINT
			{detail::texelFetch<T, P, FORMAT_RGB16_SFLOAT>::call, nullptr},				// FORMAT_RGB16_SFLOAT

			{detail::texelNorm4<T, P, u16, std::numeric_limits<T>::is_iec559>::fetch, detail::texelNorm4<T, P, u16, std::numeric_limits<T>::is_iec559>::write},	// FORMAT_RGBA16_UNORM
			{detail::texelNorm4<T, P, i16, std::numeric_limits<T>::is_iec559>::fetch, detail::texelNorm4<T, P, i16, std::numeric_limits<T>::is_iec559>::write},	// FORMAT_RGBA16_SNORM
			{detail::texelFetchCast4<T, P, glm::u16>::call, nullptr},					// FORMAT_RGBA16_USCALED
			{detail::texelFetchCast4<T, P, glm::i16>::call, nullptr},					// FORMAT_RGBA16_SSCALED
			{detail::texelFetchDefault<T, P>::call, nullptr},							// FORMAT_RGBA16_UINT
			{detail::texelFetchDefault<T, P>::call, nullptr},							// FORMAT_RGBA16_SINT
			{detail::texelFetch<T, P, FORMAT_RGBA16_SFLOAT>::call, nullptr},			// FORMAT_RGBA16_SFLOAT

			{detail::texelFetchRead1<T, P, glm::u32>::call, nullptr},					// FORMAT_R32_UINT
			{detail::texelFetchRead1<T, P, glm::i32>::call, nullptr},					// FORMAT_R32_SINT
			{detail::texelFetchRead1<T, P, glm::f32>::call, nullptr},					// FORMAT_R32_SFLOAT
			{detail::texelFetchRead2<T, P, glm::u32>::call, nullptr},					// FORMAT_RG32_UINT
			{detail::texelFetchRead2<T, P, glm::i32>::call, nullptr},					// FORMAT_RG32_SINT
			{detail::texelFetchRead2<T, P, glm::f32>::call, nullptr},					// FORMAT_RG32_SFLOAT
			{detail::texelFetchRead3<T, P, glm::u32>::call, nullptr},					// FORMAT_RGB32_UINT
			{detail::texelFetchRead3<T, P, glm::i32>::call, nullptr},					// FORMAT_RGB32_SINT
			{detail::texelFetchRead3<T, P, glm::f32>::call, nullptr},					// FORMAT_RGB32_SFLOAT
			{detail::texelFetchRead4<T, P, glm::u32>::call, nullptr},					// FORMAT_RGBA32_UINT
			{detail::texelFetchRead4<T, P, glm::i32>::call, nullptr},					// FORMAT_RGBA32_SINT
			{detail::texelFetchRead4<T, P, glm::f32>::call, nullptr},					// FORMAT_RGBA32_SFLOAT

			{detail::texelFetchRead1<T, P, glm::f64>::call, nullptr},					// FORMAT_R64_SFLOAT
			{detail::texelFetchRead2<T, P, glm::f64>::call, nullptr},					// FORMAT_RG64_SFLOAT
			{detail::texelFetchRead3<T, P, glm::f64>::call, nullptr},					// FORMAT_RGB64_SFLOAT
			{detail::texelFetchRead4<T, P, glm::f64>::call, nullptr},					// FORMAT_RGBA64_SFLOAT

			{detail::texelFetch<T, P, FORMAT_RG11B10_UFLOAT>::call, nullptr},			// FORMAT_RG11B10_UFLOAT
			{detail::texelFetch<T, P, FORMAT_RGB9E5_UFLOAT>::call, nullptr},			// FORMAT_RGB9E5_UFLOAT

			{detail::texelFetchDefault<T, P>::call, nullptr},							// FORMAT_D16_UNORM
			{detail::texelFetchDefault<T, P>::call, nullptr},							// FORMAT_D24_UNORM
			{detail::texelFetchDefault<T, P>::call, nullptr},							// FORMAT_D32_SFLOAT
			{detail::texelFetchDefault<T, P>::call, nullptr},							// FORMAT_S8_UINT
			{detail::texelFetchDefault<T, P>::call, nullptr},							// FORMAT_D16_UNORM_S8_UINT
			{detail::texelFetchDefault<T, P>::call, nullptr},							// FORMAT_D24_UNORM_S8_UINT
			{detail::texelFetchDefault<T, P>::call, nullptr},							// FORMAT_D32_SFLOAT_S8_UINT

			{detail::texelFetchDefault<T, P>::call, nullptr},							// FORMAT_RGB_DXT1_UNORM
			{detail::texelFetchDefault<T, P>::call, nullptr},							// FORMAT_RGB_DXT1_SRGB
			{detail::texelFetchDefault<T, P>::call, nullptr},							// FORMAT_RGBA_DXT1_UNORM
			{detail::texelFetchDefault<T, P>::call, nullptr},							// FORMAT_RGBA_DXT1_SRGB
			{detail::texelFetchDefault<T, P>::call, nullptr},							// FORMAT_RGBA_DXT3_UNORM
			{detail::texelFetchDefault<T, P>::call, nullptr},							// FORMAT_RGBA_DXT3_SRGB
			{detail::texelFetchDefault<T, P>::call, nullptr},							// FORMAT_RGBA_DXT5_UNORM
			{detail::texelFetchDefault<T, P>::call, nullptr},							// FORMAT_RGBA_DXT5_SRGB
			{detail::texelFetchDefault<T, P>::call, nullptr},							// FORMAT_R_ATI1N_UNORM
			{detail::texelFetchDefault<T, P>::call, nullptr},							// FORMAT_R_ATI1N_SNORM
			{detail::texelFetchDefault<T, P>::call, nullptr},							// FORMAT_RG_ATI2N_UNORM
			{detail::texelFetchDefault<T, P>::call, nullptr},							// FORMAT_RG_ATI2N_SNORM
			{detail::texelFetchDefault<T, P>::call, nullptr},							// FORMAT_RGB_BP_UFLOAT
			{detail::texelFetchDefault<T, P>::call, nullptr},							// FORMAT_RGB_BP_SFLOAT
			{detail::texelFetchDefault<T, P>::call, nullptr},							// FORMAT_RGBA_BP_UNORM
			{detail::texelFetchDefault<T, P>::call, nullptr},							// FORMAT_RGBA_BP_SRGB

			{detail::texelFetchDefault<T, P>::call, nullptr},							// FORMAT_RGB_ETC2_UNORM
			{detail::texelFetchDefault<T, P>::call, nullptr},							// FORMAT_RGB_ETC2_SRGB
			{detail::texelFetchDefault<T, P>::call, nullptr},							// FORMAT_RGBA_ETC2_A1_UNORM
			{detail::texelFetchDefault<T, P>::call, nullptr},							// FORMAT_RGBA_ETC2_A1_SRGB
			{detail::texelFetchDefault<T, P>::call, nullptr},							// FORMAT_RGBA_ETC2_UNORM
			{detail::texelFetchDefault<T, P>::call, nullptr},							// FORMAT_RGBA_ETC2_SRGB
			{detail::texelFetchDefault<T, P>::call, nullptr},							// FORMAT_R_EAC_UNORM
			{detail::texelFetchDefault<T, P>::call, nullptr},							// FORMAT_R_EAC_SNORM
			{detail::texelFetchDefault<T, P>::call, nullptr},							// FORMAT_RG_EAC_UNORM
			{detail::texelFetchDefault<T, P>::call, nullptr},							// FORMAT_RG_EAC_SNORM
			{detail::texelFetchDefault<T, P>::call, nullptr},							// FORMAT_ASTC_4x4_UNORM
			{detail::texelFetchDefault<T, P>::call, nullptr},							// FORMAT_ASTC_4x4_SRGB
			{detail::texelFetchDefault<T, P>::call, nullptr},							// FORMAT_ASTC_5x4_UNORM
			{detail::texelFetchDefault<T, P>::call, nullptr},							// FORMAT_ASTC_5x4_SRGB
			{detail::texelFetchDefault<T, P>::call, nullptr},							// FORMAT_ASTC_5x5_UNORM
			{detail::texelFetchDefault<T, P>::call, nullptr},							// FORMAT_ASTC_5x5_SRGB
			{detail::texelFetchDefault<T, P>::call, nullptr},							// FORMAT_ASTC_6x5_UNORM
			{detail::texelFetchDefault<T, P>::call, nullptr},							// FORMAT_ASTC_6x5_SRGB
			{detail::texelFetchDefault<T, P>::call, nullptr},							// FORMAT_ASTC_6x6_UNORM
			{detail::texelFetchDefault<T, P>::call, nullptr},							// FORMAT_ASTC_6x6_SRGB
			{detail::texelFetchDefault<T, P>::call, nullptr},							// FORMAT_ASTC_8x5_UNORM
			{detail::texelFetchDefault<T, P>::call, nullptr},							// FORMAT_ASTC_8x5_SRGB
			{detail::texelFetchDefault<T, P>::call, nullptr},							// FORMAT_ASTC_8x6_UNORM
			{detail::texelFetchDefault<T, P>::call, nullptr},							// FORMAT_ASTC_8x6_SRGB
			{detail::texelFetchDefault<T, P>::call, nullptr},							// FORMAT_ASTC_8x8_UNORM
			{detail::texelFetchDefault<T, P>::call, nullptr},							// FORMAT_ASTC_8x8_SRGB
			{detail::texelFetchDefault<T, P>::call, nullptr},							// FORMAT_ASTC_10x5_UNORM
			{detail::texelFetchDefault<T, P>::call, nullptr},							// FORMAT_ASTC_10x5_SRGB
			{detail::texelFetchDefault<T, P>::call, nullptr},							// FORMAT_ASTC_10x6_UNORM
			{detail::texelFetchDefault<T, P>::call, nullptr},							// FORMAT_ASTC_10x6_SRGB
			{detail::texelFetchDefault<T, P>::call, nullptr},							// FORMAT_ASTC_10x8_UNORM
			{detail::texelFetchDefault<T, P>::call, nullptr},							// FORMAT_ASTC_10x8_SRGB
			{detail::texelFetchDefault<T, P>::call, nullptr},							// FORMAT_ASTC_10x10_UNORM
			{detail::texelFetchDefault<T, P>::call, nullptr},							// FORMAT_ASTC_10x10_SRGB
			{detail::texelFetchDefault<T, P>::call, nullptr},							// FORMAT_ASTC_12x10_UNORM
			{detail::texelFetchDefault<T, P>::call, nullptr},							// FORMAT_ASTC_12x10_SRGB
			{detail::texelFetchDefault<T, P>::call, nullptr},							// FORMAT_ASTC_12x12_UNORM
			{detail::texelFetchDefault<T, P>::call, nullptr},							// FORMAT_ASTC_12x12_SRGB

			{detail::texelFetchDefault<T, P>::call, nullptr},							// FORMAT_BGRA4_UNORM
			{detail::texelFetchDefault<T, P>::call, nullptr},							// FORMAT_BGRA4_USCALED
			{detail::texelFetchDefault<T, P>::call, nullptr},							// FORMAT_B5G6R5_UNORM
			{detail::texelFetchDefault<T, P>::call, nullptr},							// FORMAT_B5G6R5_USCALED
			{detail::texelFetchDefault<T, P>::call, nullptr},							// FORMAT_BGR5A1_UNORM
			{detail::texelFetchDefault<T, P>::call, nullptr},							// FORMAT_BGR5A1_USCALED

			{detail::texelFetchNorm3<T, P, glm::u8, std::numeric_limits<T>::is_iec559>::call, nullptr},		// FORMAT_BGR8_UNORM
			{detail::texelFetchNorm3<T, P, glm::i8, std::numeric_limits<T>::is_iec559>::call, nullptr},		// FORMAT_BGR8_SNORM
			{detail::texelFetchCast3<T, P, glm::u8>::call, nullptr},											// FORMAT_BGR8_USCALED
			{detail::texelFetchCast3<T, P, glm::i8>::call, nullptr},											// FORMAT_BGR8_SSCALED
			{detail::texelFetchDefault<T, P>::call, nullptr},													// FORMAT_BGR8_UINT
			{detail::texelFetchDefault<T, P>::call, nullptr},													// FORMAT_BGR8_SINT
			{detail::texelFetchSRGB3<T, P, std::numeric_limits<T>::is_iec559>::call, nullptr},					// FORMAT_BGR8_SRGB

			{detail::texelNorm4<T, P, u8, std::numeric_limits<T>::is_iec559>::fetch, detail::texelNorm4<T, P, u8, std::numeric_limits<T>::is_iec559>::write},	// FORMAT_BGRA8_UNORM
			{detail::texelNorm4<T, P, i8, std::numeric_limits<T>::is_iec559>::fetch, detail::texelNorm4<T, P, i8, std::numeric_limits<T>::is_iec559>::write},	// FORMAT_BGRA8_SNORM
			{detail::texelFetchCast4<T, P, glm::u8>::call, nullptr},											// FORMAT_BGRA8_USCALED
			{detail::texelFetchCast4<T, P, glm::i8>::call, nullptr},											// FORMAT_BGRA8_SSCALED
			{detail::texelFetchCast4<T, P, glm::u8>::call, nullptr},											// FORMAT_BGRA8_UINT
			{detail::texelFetchCast4<T, P, glm::i8>::call, nullptr},											// FORMAT_BGRA8_SINT
			{detail::texelFetchSRGB4<T, P, std::numeric_limits<T>::is_iec559>::call, nullptr},					// FORMAT_BGRA8_SRGB

			{detail::texelFetch<T, P, FORMAT_BGR10A2_UNORM>::call, nullptr},			// FORMAT_BGR10A2_UNORM
			{detail::texelFetch<T, P, FORMAT_BGR10A2_SNORM>::call, nullptr},			// FORMAT_BGR10A2_SNORM
			{detail::texelFetch<T, P, FORMAT_BGR10A2_USCALED>::call, nullptr},			// FORMAT_BGR10A2_USCALED
			{detail::texelFetch<T, P, FORMAT_BGR10A2_SSCALED>::call, nullptr},			// FORMAT_BGR10A2_SSCALED
			{detail::texelFetch<T, P, FORMAT_BGR10A2_UINT>::call, nullptr},			// FORMAT_BGR10A2_UINT
			{detail::texelFetch<T, P, FORMAT_BGR10A2_SINT>::call, nullptr},			// FORMAT_BGR10A2_SINT

			{detail::texelFetchDefault<T, P>::call, nullptr},							// FORMAT_RG3B2_UNORM
			{detail::texelNorm4<T, P, u8, std::numeric_limits<T>::is_iec559>::fetch, detail::texelNorm4<T, P, u8, std::numeric_limits<T>::is_iec559>::write},	// FORMAT_BGRX8_UNORM
			{detail::texelFetchSRGB4<T, P, std::numeric_limits<T>::is_iec559>::call, nullptr},					// FORMAT_BGRX8_SRGB

			{detail::texelFetchNorm1<T, P, glm::u8, std::numeric_limits<T>::is_iec559>::call, nullptr},		// FORMAT_L8_UNORM
			{detail::texelFetchNorm1<T, P, glm::u8, std::numeric_limits<T>::is_iec559>::call, nullptr},		// FORMAT_A8_UNORM
			{detail::texelFetchNorm2<T, P, glm::u8, std::numeric_limits<T>::is_iec559>::call, nullptr},		// FORMAT_LA8_UNORM
			{detail::texelFetchNorm1<T, P, glm::u16, std::numeric_limits<T>::is_iec559>::call, nullptr},		// FORMAT_L16_UNORM
			{detail::texelFetchNorm1<T, P, glm::u16, std::numeric_limits<T>::is_iec559>::call, nullptr},		// FORMAT_A16_UNORM
			{detail::texelFetchNorm2<T, P, glm::u16, std::numeric_limits<T>::is_iec559>::call, nullptr},		// FORMAT_LA16_UNORM

			{detail::texelFetchDefault<T, P>::call, nullptr},							// FORMAT_RGB_PVRTC1_8X8_UNORM
			{detail::texelFetchDefault<T, P>::call, nullptr},							// FORMAT_RGB_PVRTC1_8X8_SRGB
			{detail::texelFetchDefault<T, P>::call, nullptr},							// FORMAT_RGB_PVRTC1_16X8_UNORM
			{detail::texelFetchDefault<T, P>::call, nullptr},							// FORMAT_RGB_PVRTC1_16X8_SRGB
			{detail::texelFetchDefault<T, P>::call, nullptr},							// FORMAT_RGBA_PVRTC1_8X8_UNORM
			{detail::texelFetchDefault<T, P>::call, nullptr},							// FORMAT_RGBA_PVRTC1_8X8_SRGB
			{detail::texelFetchDefault<T, P>::call, nullptr},							// FORMAT_RGBA_PVRTC1_16X8_UNORM
			{detail::texelFetchDefault<T, P>::call, nullptr},							// FORMAT_RGBA_PVRTC1_16X8_SRGB
			{detail::texelFetchDefault<T, P>::call, nullptr},							// FORMAT_RGBA_PVRTC2_4X4_UNORM
			{detail::texelFetchDefault<T, P>::call, nullptr},							// FORMAT_RGBA_PVRTC2_4X4_SRGB
			{detail::texelFetchDefault<T, P>::call, nullptr},							// FORMAT_RGBA_PVRTC2_8X4_UNORM
			{detail::texelFetchDefault<T, P>::call, nullptr},							// FORMAT_RGBA_PVRTC2_8X4_SRGB

			{detail::texelFetchDefault<T, P>::call, nullptr},							// FORMAT_RGB_ETC_UNORM
			{detail::texelFetchDefault<T, P>::call, nullptr},							// FORMAT_RGB_ATC_UNORM
			{detail::texelFetchDefault<T, P>::call, nullptr},							// FORMAT_RGBA_ATC_EXPLICIT_UNORM
			{detail::texelFetchDefault<T, P>::call, nullptr},							// FORMAT_RGBA_ATC_INTERPOLATED_UNORM
		};
		static_assert(sizeof(Table) / sizeof(Table[0]) == FORMAT_COUNT, "Texel functions need to be updated");

		return Table[Format - FORMAT_FIRST];
	};
}//namespace gli

