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
	inline sampler2D<T, P>::sampler2D(texture2D const & Texture, wrap Wrap, filter Mip, filter Min, texel_type const & BorderColor)
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
	inline typename sampler2D<T, P>::texel_type sampler2D<T, P>::texel_fetch(texelcoord_type const & TexelCoord, size_type const & Level) const
	{
		assert(this->Convert.Fetch);
		return this->Convert.Fetch(this->Texture, TexelCoord, 0, 0, Level);
	}

	template <typename T, glm::precision P>
	inline void sampler2D<T, P>::texel_write(texelcoord_type const & TexelCoord, size_type const & Level, texel_type const & Texel)
	{
		assert(this->Convert.Write);
		this->Convert.Write(this->Texture, TexelCoord, 0, 0, Level, Texel);
	}

	template <typename T, precision P>
	inline void sampler2D<T, P>::clear(texel_type const & Color)
	{
		detail::clear<texture_type, T, P>::call(this->Texture, this->Convert.Write, Color);
	}

	template <typename T, glm::precision P>
	inline typename sampler2D<T, P>::texel_type sampler2D<T, P>::texture_lod(samplecoord_type const & Texcoord, float Level) const
	{
		assert(std::numeric_limits<T>::is_iec559);

		samplecoord_type const TexcoordWrap(this->WrapFunc(Texcoord.x), this->WrapFunc(Texcoord.y));

		size_type const minLevel = size_type(floor(Level));
		size_type const maxLevel = size_type(ceil(Level));

		if (this->Mip == FILTER_LINEAR)
		{
			texel_type const minTexel = this->Min == FILTER_LINEAR ? this->texture_lod_linear(TexcoordWrap, minLevel) : this->texture_lod_nearest(TexcoordWrap, minLevel);
			texel_type const maxTexel = this->Min == FILTER_LINEAR ? this->texture_lod_linear(TexcoordWrap, maxLevel) : this->texture_lod_nearest(TexcoordWrap, minLevel);
			return mix(minTexel, maxTexel, fract(Level));
		} else
		{
			texture2D::size_type const level = texture2D::size_type(round(Level));
			return this->Min == FILTER_LINEAR ? this->texture_lod_linear(TexcoordWrap, level) : this->texture_lod_nearest(TexcoordWrap, minLevel);
		}
	}

	template <typename T, precision P>
	inline typename sampler2D<T, P>::texel_type sampler2D<T, P>::texture_lod_linear(samplecoord_type const & SampleCoord, size_type Level) const
	{
		assert(std::numeric_limits<T>::is_iec559);
		assert(this->Convert.Fetch);

		texelcoord_type const TexelDim(this->Texture.dimensions(Level));
		samplecoord_type const TexelDimF(TexelDim);
		samplecoord_type const TexelLast(TexelDimF - static_cast<T>(1));

		samplecoord_type const ScaledCoordFloor(floor(SampleCoord * TexelLast));
		samplecoord_type const ScaledCoordCeil(ceil(SampleCoord * TexelLast));

		texelcoord_type const TexelCoordFloor(ScaledCoordFloor);
		texelcoord_type const TexelCoordCeil(ScaledCoordCeil);

		bvec2 const UseTexelFloor(detail::in_interval(TexelCoordFloor, texelcoord_type(0), TexelDim - 1));
		bvec2 const UseTexelCeil(detail::in_interval(TexelCoordCeil, texelcoord_type(0), TexelDim - 1));

		texel_type Texel00(this->BorderColor);
		if(UseTexelFloor.s && UseTexelFloor.t)
			Texel00 = this->Convert.Fetch(this->Texture, texelcoord_type(TexelCoordFloor.s, TexelCoordFloor.t), 0, 0, Level);

		texel_type Texel10(this->BorderColor);
		if(UseTexelCeil.s && UseTexelFloor.t)
			Texel10 = this->Convert.Fetch(this->Texture, texelcoord_type(TexelCoordCeil.s, TexelCoordFloor.t), 0, 0, Level);

		texel_type Texel11(this->BorderColor);
		if(UseTexelCeil.s && UseTexelCeil.t)
			Texel11 = this->Convert.Fetch(this->Texture, texelcoord_type(TexelCoordCeil.s, TexelCoordCeil.t), 0, 0, Level);

		texel_type Texel01(this->BorderColor);
		if(UseTexelFloor.s && TexelCoordCeil.t)
			Texel01 = this->Convert.Fetch(this->Texture, texelcoord_type(TexelCoordFloor.s, TexelCoordCeil.t), 0, 0, Level);

		samplecoord_type const BlendFactor((SampleCoord - ScaledCoordFloor / TexelLast) * TexelLast);

		texel_type const ValueA(mix(Texel00, Texel10, BlendFactor.s));
		texel_type const ValueB(mix(Texel01, Texel11, BlendFactor.s));
		texel_type const Texel(mix(ValueA, ValueB, BlendFactor.t));
		return Texel;
	}

	template <typename T, precision P>
	inline typename sampler2D<T, P>::texel_type sampler2D<T, P>::texture_lod_nearest(samplecoord_type const & SampleCoord, size_type Level) const
	{
		assert(std::numeric_limits<T>::is_iec559);

		texelcoord_type const TexelDim = this->Texture.dimensions(Level);

		samplecoord_type const TexelLast(samplecoord_type(TexelDim) - static_cast<T>(1));
		texelcoord_type const SampleCoordFloor(floor(SampleCoord * TexelLast));

		bool const UseBorderColor = SampleCoordFloor.s > static_cast<int>(TexelDim.x) || SampleCoordFloor.s < 0 || SampleCoordFloor.t > static_cast<int>(TexelDim.y) || SampleCoordFloor.t < 0;

		return UseBorderColor ? this->BorderColor : this->texel_fetch(texelcoord_type(SampleCoordFloor), Level);
	}

	template <typename T, precision P>
	inline void sampler2D<T, P>::generate_mipmaps()
	{
		assert(!this->Texture.empty());
		assert(!is_compressed(this->Texture.format()));

		this->generate_mipmaps(this->Texture.base_level(), this->Texture.max_level());
	}

	template <typename T, precision P>
	inline void sampler2D<T, P>::generate_mipmaps(size_type BaseLevel, size_type MaxLevel)
	{
		assert(!this->Texture.empty());
		assert(!is_compressed(this->Texture.format()));
		assert(this->Texture.max_level() >= MaxLevel);
		assert(BaseLevel <= MaxLevel);

		for(size_type Level = BaseLevel; Level < MaxLevel; ++Level)
		{
			samplecoord_type const SampleCoordScale(static_cast<T>(1) / samplecoord_type(this->Texture.dimensions(Level + 0)));
			texelcoord_type const DimDst = this->Texture.dimensions(Level + 1);

			for(size_type j = 0; j < DimDst.y; ++j)
			for(size_type i = 0; i < DimDst.x; ++i)
			{
				size_type const x = (i << 1);
				size_type const y = (j << 1);

				texel_type const Texel00 = this->texture_lod(samplecoord_type(x + 0, y + 0) * SampleCoordScale, static_cast<T>(Level));
				texel_type const Texel01 = this->texture_lod(samplecoord_type(x + 0, y + 1) * SampleCoordScale, static_cast<T>(Level));
				texel_type const Texel11 = this->texture_lod(samplecoord_type(x + 1, y + 1) * SampleCoordScale, static_cast<T>(Level));
				texel_type const Texel10 = this->texture_lod(samplecoord_type(x + 1, y + 0) * SampleCoordScale, static_cast<T>(Level));

				texel_type const Texel = (Texel00 + Texel01 + Texel11 + Texel10) * static_cast<T>(0.25);
				this->texel_write(texelcoord_type(i, j), Level + 1, Texel);
			}
		}
	}
}//namespace gli

