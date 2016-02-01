#pragma once

#include "../gli.hpp"

namespace gli
{
	/// Fetch a texel from a texture
	/// The texture format must be uncompressed
	template <typename genType>
	genType texel_fetch(
		texture2d const & Texture,
		texture2d::dim_type const & TexelCoord,
		texture2d::size_type const & Level);

	/// Write a texel to a texture
	/// The texture format must be uncompressed
	template <typename genType>
	void texel_write(
		texture2d & Texture,
		texture2d::dim_type const & TexelCoord,
		texture2d::size_type const & Level,
		genType const & Color);

	/// Sample a pixel from a texture
	/// The texture format must be uncompressed
	template <typename genType>
	genType texture_lod(
		texture2d const & Texture,
		texture2d::samplecoord_type const & SampleCoord,
		texture2d::size_type const & Level);

}//namespace gli

#include "fetch.inl"
