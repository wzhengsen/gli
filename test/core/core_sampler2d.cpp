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
#include <ctime>

namespace gli{
namespace detail
{
	template <typename genType>
	genType textureLodLinear(genType const * const TexelData, texture2D::dim_type const & TexelDim, texture2D::texcoord_type const & Texcoord, genType const & BorderColor)
	{
		int const s_below = int(glm::floor(Texcoord.s * float(TexelDim.x - 1)));
		int const s_above = int(glm::ceil( Texcoord.s * float(TexelDim.x - 1)));
		int const t_below = int(glm::floor(Texcoord.t * float(TexelDim.y - 1)));
		int const t_above = int(glm::ceil( Texcoord.t * float(TexelDim.y - 1)));

		glm::bvec4 UseBorderColor(
			s_below < 0 || s_below > static_cast<int>(TexelDim.x),
			s_above < 0 || s_above > static_cast<int>(TexelDim.x),
			t_below < 0 || t_below > static_cast<int>(TexelDim.y),
			t_above < 0 || t_above > static_cast<int>(TexelDim.y));

		float const s_below_normalized = s_below / float(TexelDim.x);
		float const t_below_normalized = t_below / float(TexelDim.y);

		genType const Value1 = UseBorderColor[0] || UseBorderColor[2] ? BorderColor : TexelData[s_below + t_below * TexelDim.x];
		genType const Value2 = UseBorderColor[1] || UseBorderColor[2] ? BorderColor : TexelData[s_above + t_below * TexelDim.x];
		genType const Value3 = UseBorderColor[1] || UseBorderColor[3] ? BorderColor : TexelData[s_above + t_above * TexelDim.x];
		genType const Value4 = UseBorderColor[0] || UseBorderColor[3] ? BorderColor : TexelData[s_below + t_above * TexelDim.x];

		float const BlendA = float(Texcoord.s - s_below_normalized) * float(TexelDim.x - 1);
		float const BlendB = float(Texcoord.s - s_below_normalized) * float(TexelDim.x - 1);
		float const BlendC = float(Texcoord.t - t_below_normalized) * float(TexelDim.y - 1);

		genType const ValueA(glm::mix(Value1, Value2, BlendA));
		genType const ValueB(glm::mix(Value4, Value3, BlendB));

		return genType(glm::mix(ValueA, ValueB, BlendC));
	}

	template <typename genType>
	genType textureLodNearest(genType const * const TexelData, texture2D::dim_type const & TexelDim, texture2D::texcoord_type const & Texcoord, genType const & BorderColor)
	{
		int const s = int(glm::floor(Texcoord.s * float(TexelDim.x - 1)));
		int const t = int(glm::floor(Texcoord.t * float(TexelDim.y - 1)));

		return s > static_cast<int>(TexelDim.x) || s < 0 || t > static_cast<int>(TexelDim.y) || t < 0 ? BorderColor : TexelData[s + t * TexelDim.x];
	}

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

	template <typename genType>
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
		{}

	protected:
		wrap WrapMode;
		wrapFunc WrapFunc;
		filter Mip;
	};

	template <typename genType>
	class sampler2D : public sampler<genType>
	{
		typedef genType (*textureLodFunc)(genType const * const TexelData, texture2D::dim_type const & TexelDim, texture2D::texcoord_type const & Texcoord, genType const & BorderColor);

		textureLodFunc getFunc(filter Filter) const
		{
			static textureLodFunc Table[] =
			{
				nullptr,
				detail::textureLodNearest,
				detail::textureLodLinear
			};
			static_assert(sizeof(Table) / sizeof(Table[0]) == FILTER_COUNT, "Table needs to be updated");

			return Table[Filter];
		}

		struct cache
		{
			genType* Data;
			texture2D::size_type Size;
			texture2D::dim_type Dim;
		};

	public:
		sampler2D(texture Texture, wrap Wrap, filter Mip, filter Min, genType const & BorderColor/*, filter Mag, filter Min, filter Mip*/)
			: sampler<genType>(Wrap, Texture.levels() > 1 ? Mip : FILTER_NEAREST, Min)
			, Texture(Texture)
			, Caches(Texture.levels())
			, TextureLodFunc(getFunc(Min))
			, BorderColor(BorderColor)
		{
			assert(!Texture.empty());
			assert(!is_compressed(Texture.format()));

			for(std::size_t Level = 0; Level < this->Caches.size(); ++Level)
			{
				this->Caches[Level].Data = Texture.data<genType>(0, 0, Level);
				this->Caches[Level].Size = Texture.size<genType>(Level);
				this->Caches[Level].Dim = texture2D::dim_type(Texture.dimensions(Level));
			}
		}

		genType texel_fetch(texture2D::dim_type const & TexelCoord, texture2D::size_type const & Level)
		{
			std::size_t const Offset = TexelCoord.x + TexelCoord.y * this->Caches[Level].Dim.x;
			assert(Index < this->Caches[Level].Size);

			return *(this->Caches[Level].Data + Offset);
		}

		void texel_write(texture2D::dim_type const & TexelCoord, texture2D::size_type const & Level, genType const & Color)
		{
			std::size_t const Offset = TexelCoord.x + TexelCoord.y * this->Caches[Level].Dim.x;
			assert(Offset < this->Caches[Level].Size);

			*(this->Caches[Level].Data + Offset) = Color;
		}

		genType texture_lod(texture2D::texcoord_type const & Texcoord, float Level)
		{
			texture2D::texcoord_type const TexcoordWrap(this->WrapFunc(Texcoord.x), this->WrapFunc(Texcoord.y));

			int const minLod = int(glm::floor(Level));
			int const maxLod = int(glm::ceil(Level));

			if(this->Mip == FILTER_LINEAR)
			{
				genType const minTexel = this->TextureLodFunc(this->Caches[minLod].Data, this->Caches[minLod].Dim, TexcoordWrap, this->BorderColor);
				genType const maxTexel = this->TextureLodFunc(this->Caches[maxLod].Data, this->Caches[maxLod].Dim, TexcoordWrap, this->BorderColor);
				return genType(mix(minTexel, maxTexel, glm::fract(Level)));
			}
			else
			{
				int const lod = int(glm::round(Level));
				return genType(this->TextureLodFunc(this->Caches[lod].Data, this->Caches[lod].Dim, TexcoordWrap, this->BorderColor));
			}
		}

	private:
		texture Texture;
		std::vector<cache> Caches;
		textureLodFunc TextureLodFunc;
		genType BorderColor;
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

		glm::u8vec4 Data0 = Texture.fetch<glm::u8vec4>(gli::texture2D::dim_type(0, 0), 0);
		glm::u8vec4 Data1 = Texture.fetch<glm::u8vec4>(gli::texture2D::dim_type(1, 0), 0);
		glm::u8vec4 Data2 = Texture.fetch<glm::u8vec4>(gli::texture2D::dim_type(2, 0), 0);
		glm::u8vec4 Data3 = Texture.fetch<glm::u8vec4>(gli::texture2D::dim_type(3, 0), 0);
		glm::u8vec4 Data4 = Texture.fetch<glm::u8vec4>(gli::texture2D::dim_type(0, 1), 0);
		glm::u8vec4 Data5 = Texture.fetch<glm::u8vec4>(gli::texture2D::dim_type(1, 1), 0);
		glm::u8vec4 Data6 = Texture.fetch<glm::u8vec4>(gli::texture2D::dim_type(2, 1), 0);
		glm::u8vec4 Data7 = Texture.fetch<glm::u8vec4>(gli::texture2D::dim_type(3, 1), 0);

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
			gli::sampler2D<glm::u8vec4> Sampler(TextureB, gli::WRAP_CLAMP_TO_EDGE, gli::FILTER_LINEAR, gli::FILTER_LINEAR, glm::u8vec4(0, 127, 255, 255));

			{
				std::clock_t TimeStart = std::clock();

				for(std::size_t y = 0; y < Size.y; ++y)
				for(std::size_t x = 0; x < Size.x; ++x)
				{
					Sampler.texel_write(gli::texture2D::dim_type(x, y), 0, glm::u8vec4(255, 128,   0, 255));
				}

				std::clock_t TimeEnd = std::clock();

				printf("sampler2D::texel_write - Time: %lu\n", TimeEnd - TimeStart);
			}

			{
				std::clock_t TimeStart = std::clock();

				for(float y = -0.5f; y < 1.5f; y += 0.025f)
				for(float x = -0.5f; x < 1.5f; x += 0.025f)
				{
					glm::u8vec4 Color = Sampler.texture_lod(gli::texture2D::texcoord_type(x, y), 0);

					Error += glm::all(glm::equal(Color, glm::u8vec4(255, 128,   0, 255))) ? 0 : 1;
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

		glm::u8vec4 const Orange(255, 127, 0, 255);
		glm::u8vec4 const Blue(0, 127, 255, 255);

		gli::texture2D::dim_type const Size(32);
		gli::texture2D Texture(gli::FORMAT_RGBA8_UNORM, Size, 1);
		Texture.clear(Orange);

		gli::sampler2D<glm::u8vec4> Sampler(Texture, gli::WRAP_CLAMP_TO_BORDER, gli::FILTER_LINEAR, gli::FILTER_LINEAR, Blue);

		{
			glm::u8vec4 const Color = Sampler.texture_lod(gli::vec2(0.5f, 0.5f), 0.0f);
			Error += glm::all(glm::equal(Color, Orange)) ? 0 : 1;
		}
		{
			glm::u8vec4 const Color = Sampler.texture_lod(gli::vec2(-0.5f, -0.5f), 0.0f);
			Error += glm::all(glm::equal(Color, Blue)) ? 0 : 1;
		}
		{
			glm::u8vec4 const Color = Sampler.texture_lod(gli::vec2(1.5f,-0.5f), 0.0f);
			Error += glm::all(glm::equal(Color, Blue)) ? 0 : 1;
		}
		{
			glm::u8vec4 const Color = Sampler.texture_lod(gli::vec2(1.5f, 1.5f), 0.0f);
			Error += glm::all(glm::equal(Color, Blue)) ? 0 : 1;
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

