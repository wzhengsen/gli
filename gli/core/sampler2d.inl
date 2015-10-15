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

#include "clear.hpp"
#include <glm/vector_relational.hpp>

namespace gli{
namespace detail
{
	template <typename T, precision P, template <typename, glm::precision> class vecType>
	inline vecType<bool, P> in_interval(vecType<T, P> const & Value, vecType<T, P> const & Min, vecType<T, P> const & Max)
	{
		return greaterThanEqual(Value, Min) && lessThanEqual(Value, Max);
	}
}//namespace detail

	template <typename T, precision P>
	inline sampler2D<T, P>::sampler2D(texture2D const & Texture, wrap Wrap, filter Mip, filter Min, tvec4<T, P> const & BorderColor)
		: sampler(Wrap, Texture.levels() > 1 ? Mip : FILTER_NEAREST, Min)
		, Texture(Texture)
		, Convert(detail::convert<texture2D, T, P>::call(this->Texture.format()))
		, BorderColor(BorderColor)
	{
		assert(!Texture.empty());
		assert(!is_compressed(Texture.format()));
	}

	template <typename T, precision P>
	inline texture2D const & sampler2D<T, P>::operator()() const
	{
		return this->Texture;
	}

	template <typename T, glm::precision P>
	inline tvec4<T, P> sampler2D<T, P>::texel_fetch(dim_type const & TexelCoord, size_type const & Level) const
	{
		assert(this->Convert.Fetch);
		return this->Convert.Fetch(this->Texture, TexelCoord, 0, 0, Level);
	}

	template <typename T, glm::precision P>
	inline void sampler2D<T, P>::texel_write(dim_type const & TexelCoord, size_type const & Level, tvec4<T, P> const & Texel)
	{
		assert(this->Convert.Write);
		this->Convert.Write(this->Texture, TexelCoord, 0, 0, Level, Texel);
	}

	template <typename T, precision P>
	inline void sampler2D<T, P>::clear(tvec4<T, P> const & Color)
	{
		detail::clear<texture_type, T, P>::call(this->Texture, this->Convert.Write, Color);
	}

	template <typename T, glm::precision P>
	inline tvec4<T, P> sampler2D<T, P>::texture_lod(samplecoord_type const & Texcoord, float Level) const
	{
		assert(std::numeric_limits<T>::is_iec559);

		samplecoord_type const TexcoordWrap(this->WrapFunc(Texcoord.x), this->WrapFunc(Texcoord.y));

		size_type const minLevel = size_type(floor(Level));
		size_type const maxLevel = size_type(ceil(Level));

		if (this->Mip == FILTER_LINEAR)
		{
			tvec4<T, P> const minTexel = this->Min == FILTER_LINEAR ? this->texture_lod_linear(TexcoordWrap, minLevel) : this->texture_lod_nearest(TexcoordWrap, minLevel);
			tvec4<T, P> const maxTexel = this->Min == FILTER_LINEAR ? this->texture_lod_linear(TexcoordWrap, maxLevel) : this->texture_lod_nearest(TexcoordWrap, minLevel);
			return mix(minTexel, maxTexel, fract(Level));
		} else
		{
			texture2D::size_type const level = texture2D::size_type(round(Level));
			return this->Min == FILTER_LINEAR ? this->texture_lod_linear(TexcoordWrap, level) : this->texture_lod_nearest(TexcoordWrap, minLevel);
		}
	}

	template <typename T, precision P>
	inline tvec4<T, P> sampler2D<T, P>::texture_lod_linear(samplecoord_type const & SampleCoord, size_type Level) const
	{
		assert(std::numeric_limits<T>::is_iec559);

		dim_type const TexelDim(this->Texture.dimensions(Level));
		tvec2<T, P> const TexelDimF(TexelDim);
		tvec2<T, P> const TexelLast(TexelDimF - static_cast<T>(1));

		tvec2<float, P> const ScaledCoordFloor(floor(SampleCoord * TexelLast));
		tvec2<float, P> const ScaledCoordCeil(ceil(SampleCoord * TexelLast));

		tvec2<int, P> const TexelCoordFloor(ScaledCoordFloor);
		tvec2<int, P> const TexelCoordCeil(ScaledCoordCeil);

		bvec2 const UseTexelFloor(detail::in_interval(TexelCoordFloor, tvec2<int, P>(0), tvec2<int, P>(TexelDim) - 1));
		bvec2 const UseTexelCeil(detail::in_interval(TexelCoordCeil, tvec2<int, P>(0), tvec2<int, P>(TexelDim) - 1));

		tvec4<T, P> Texel00(this->BorderColor);
		if(UseTexelFloor.s && UseTexelFloor.t)
			Texel00 = this->texel_fetch(dim_type(TexelCoordFloor.s, TexelCoordFloor.t), Level);

		tvec4<T, P> Texel10(this->BorderColor);
		if(UseTexelCeil.s && UseTexelFloor.t)
			Texel10 = this->texel_fetch(dim_type(TexelCoordCeil.s, TexelCoordFloor.t), Level);

		tvec4<T, P> Texel11(this->BorderColor);
		if(UseTexelCeil.s && UseTexelCeil.t)
			Texel11 = this->texel_fetch(dim_type(TexelCoordCeil.s, TexelCoordCeil.t), Level);

		tvec4<T, P> Texel01(this->BorderColor);
		if(UseTexelFloor.s && TexelCoordCeil.t)
			Texel01 = this->texel_fetch(dim_type(TexelCoordFloor.s, TexelCoordCeil.t), Level);

		tvec2<T, P> const BlendFactor((SampleCoord - ScaledCoordFloor / TexelLast) * TexelLast);

		tvec4<T, P> const ValueA(mix(Texel00, Texel10, BlendFactor.s));
		tvec4<T, P> const ValueB(mix(Texel01, Texel11, BlendFactor.s));
		tvec4<T, P> const Texel(mix(ValueA, ValueB, BlendFactor.t));
		return Texel;
	}

	template <typename T, precision P>
	inline tvec4<T, P> sampler2D<T, P>::texture_lod_nearest(samplecoord_type const & Texcoord, size_type Level) const
	{
		assert(std::numeric_limits<T>::is_iec559);

		dim_type const TexelDim = this->Texture.dimensions(Level);

		int const s = int(floor(Texcoord.s * static_cast<T>(TexelDim.x - 1)));
		int const t = int(floor(Texcoord.t * static_cast<T>(TexelDim.y - 1)));

		bool const UseBorderColor = s > static_cast<int>(TexelDim.x) || s < 0 || t > static_cast<int>(TexelDim.y) || t < 0;

		return UseBorderColor ? this->BorderColor : this->texel_fetch(dim_type(s, t), Level);
	}

	template <typename T, precision P>
	inline void sampler2D<T, P>::generate_mipmaps()
	{
		assert(!this->Texture.empty());
		assert(!is_compressed(this->Texture.format()));

		this->generate_mipmaps(this->Texture.base_level(), this->Texture.max_level());
	}

	template <typename T, precision P>
	inline void sampler2D<T, P>::generate_mipmaps(typename sampler2D<T, P>::size_type BaseLevel, typename sampler2D<T, P>::size_type MaxLevel)
	{
		assert(!this->Texture.empty());
		assert(!is_compressed(this->Texture.format()));
		assert(this->Texture.max_level() >= MaxLevel);
		assert(BaseLevel <= MaxLevel);

		for(size_type Level = BaseLevel; Level < MaxLevel; ++Level)
		{
			samplecoord_type const SampleCoordScale(static_cast<T>(1) / samplecoord_type(this->Texture.dimensions(Level + 0)));
			dim_type const DimDst = this->Texture.dimensions(Level + 1);

			for(size_t j = 0; j < DimDst.y; ++j)
			for(size_t i = 0; i < DimDst.x; ++i)
			{
				size_t const x = (i << 1);
				size_t const y = (j << 1);

				tvec4<T, P> Texel00 = this->texture_lod(samplecoord_type(x + 0, y + 0) * SampleCoordScale, static_cast<float>(Level));
				tvec4<T, P> Texel01 = this->texture_lod(samplecoord_type(x + 0, y + 1) * SampleCoordScale, static_cast<float>(Level));
				tvec4<T, P> Texel11 = this->texture_lod(samplecoord_type(x + 1, y + 1) * SampleCoordScale, static_cast<float>(Level));
				tvec4<T, P> Texel10 = this->texture_lod(samplecoord_type(x + 1, y + 0) * SampleCoordScale, static_cast<float>(Level));

				tvec4<T, P> const Texel = (Texel00 + Texel01 + Texel11 + Texel10) * static_cast<float>(0.25);
				this->texel_write(dim_type(i, j), Level + 1, Texel);
			}
		}
	}
}//namespace gli

