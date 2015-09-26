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
#include <ctime>

namespace gli{
namespace detail
{
	inline float passThrought(float const & texcoord)
	{
		return texcoord;
	}
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
		//typedef glm::tvec4<floatType, P> (*textureLodFunc)(texture2D::texcoord_type const & Texcoord, texture2D::size_type Level);

	public:
		sampler2D(texture2D const & Texture, wrap Wrap, filter Mip, filter Min, glm::tvec4<floatType, P> const & BorderColor)
			: sampler(Wrap, Texture.levels() > 1 ? Mip : FILTER_NEAREST, Min)
			, Texture(Texture)
			, BorderColor(BorderColor)
		{
			static_assert(std::numeric_limits<floatType>::is_iec559, "'sampler2D' accepts only floating-point types for 'floatType' template parameter");
			assert(!Texture.empty());
			assert(!is_compressed(Texture.format()));
		}

		template <template <typename, glm::precision> class vecType, typename valType>
		glm::tvec4<floatType, P> texel_fetch(texture2D::dim_type const & TexelCoord, texture2D::size_type const & Level) const
		{
			vecType<floatType, P> Texel = glm::compNormalize<floatType>(this->Texture.load<vecType<valType, P> >(TexelCoord, Level));

			if(gli::is_srgb(Texture.format()))
				Texel = glm::convertSRGBToLinear(Texel);

			glm::tvec4<floatType, P> Result(0.0f, 0.0f, 0.0f, 1.0f);
			for(glm::length_t i = 0, n = Texel.length(); i < n; ++i)
				Result[i] = Texel[i];
			return Result;
		}

		template <template <typename, glm::precision> class vecType, typename valType>
		void texel_write(texture2D::dim_type const & TexelCoord, texture2D::size_type const & Level, glm::tvec4<floatType, P> const & Texel)
		{
			vecType<floatType, P> TexelTmp(Texel);

			if(gli::is_srgb(Texture.format()))
				TexelTmp = glm::convertLinearToSRGB(TexelTmp);

			vecType<valType, P> const storedTexel(glm::compScale<valType>(TexelTmp));

			this->Texture.store<vecType<valType, P> >(TexelCoord, Level, storedTexel);
		}

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

		texture2D Texture;
		glm::tvec4<floatType, P> BorderColor;
	};
}//namespace gli

namespace fetch
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
}//namespace fetch

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

int main()
{
	int Error(0);

	Error += clamp_to_border::test();
	Error += fetch::test();
	Error += sampler::test();

	return Error;
}

