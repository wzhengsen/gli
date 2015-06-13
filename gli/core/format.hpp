///////////////////////////////////////////////////////////////////////////////////
/// OpenGL Image (gli.g-truc.net)
///
/// Copyright (c) 2008 - 2012 G-Truc Creation (www.g-truc.net)
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
/// @file gli/core/format.hpp
/// @date 2012-10-16 / 2012-10-16
/// @author Christophe Riccio
///////////////////////////////////////////////////////////////////////////////////

#pragma once

namespace gli
{
	enum format
	{
		FORMAT_INVALID = -1,

		// unorm formats
		R8_UNORM = 0, FORMAT_FIRST = R8_UNORM,
		RG8_UNORM,
		RGB8_UNORM,
		RGBA8_UNORM,

		R16_UNORM,
		RG16_UNORM,
		RGB16_UNORM,
		RGBA16_UNORM,

		RGB10A2_UNORM,

		// snorm formats
		R8_SNORM,
		RG8_SNORM,
		RGB8_SNORM,
		RGBA8_SNORM,

		R16_SNORM,
		RG16_SNORM,
		RGB16_SNORM,
		RGBA16_SNORM,

		// Unsigned integer formats
		R8U,
		RG8U,
		RGB8U,
		RGBA8U,

		R16U,
		RG16U,
		RGB16U,
		RGBA16U,

		R32U,
		RG32U,
		RGB32U,
		RGBA32U,

		RGB10A2U,

		// Signed integer formats
		R8I,
		RG8I,
		RGB8I,
		RGBA8I,

		R16I,
		RG16I,
		RGB16I,
		RGBA16I,

		R32I,
		RG32I,
		RGB32I,
		RGBA32I,

		// Floating formats
		R16F,
		RG16F,
		RGB16F,
		RGBA16F,

		R32F,
		RG32F,
		RGB32F,
		RGBA32F,

		// sRGB formats
		SR8_UNORM,
		SRG8_UNORM,
		SRGB8_UNORM,
		SRGB8_ALPHA8_UNORM,

		// Packed formats
		RGB9E5,
		RG11B10F,
		R3G3B2_UNORM,
		R5G6B5_UNORM,
		RGB5A1_UNORM,
		RGBA4_UNORM,

		// Swizzle formats
		BGR8_UNORM,
		BGRA8_UNORM,

		// Luminance Alpha formats
		L8_UNORM,
		A8_UNORM,
		LA8_UNORM,
		L16_UNORM,
		A16_UNORM,
		LA16_UNORM,

		// Depth formats
		D16,
		D24X8,
		D24S8,
		D32F,
		D32FS8X24,

		// Compressed formats
		RGB_DXT1_UNORM, FORMAT_COMPRESSED_FIRST = RGB_DXT1_UNORM,
		RGBA_DXT1_UNORM,
		RGBA_DXT3_UNORM,
		RGBA_DXT5_UNORM,
		R_ATI1N_UNORM,
		R_ATI1N_SNORM,
		RG_ATI2N_UNORM,
		RG_ATI2N_SNORM,
		RGB_BP_UFLOAT,
		RGB_BP_SFLOAT,
		RGB_BP_UNORM,
		RGB_PVRTC_4BPPV1_UNORM,
		RGB_PVRTC_2BPPV1_UNORM,
		RGBA_PVRTC_4BPPV1_UNORM,
		RGBA_PVRTC_2BPPV1_UNORM,
		ATC_RGB_UNORM,
		ATC_RGBA_EXPLICIT_ALPHA_UNORM,
		ATC_RGBA_INTERPOLATED_ALPHA_UNORM,
		RGB_ETC2_UNORM,
		RGB_ETC2_PUNCHTHROUGH_ALPHA_UNORM,
		RGBA_ETC2_UNORM,
		R11_EAC_UNORM,
		R11_EAC_SNORM,
		RG11_EAC_UNORM,
		RG11_EAC_SNORM,
		RGBA_ASTC_4x4,
		RGBA_ASTC_5x4,
		RGBA_ASTC_5x5,
		RGBA_ASTC_6x5,
		RGBA_ASTC_6x6,
		RGBA_ASTC_8x5,
		RGBA_ASTC_8x6,
		RGBA_ASTC_8x8,
		RGBA_ASTC_10x5,
		RGBA_ASTC_10x6,
		RGBA_ASTC_10x8,
		RGBA_ASTC_10x10,
		RGBA_ASTC_12x10,
		RGBA_ASTC_12x12,

		// Compressed sRGB formats
		SRGB_DXT1_UNORM,
		SRGB_ALPHA_DXT1_UNORM,
		SRGB_ALPHA_DXT3_UNORM,
		SRGB_ALPHA_DXT5_UNORM,
		SRGB_BP_UNORM,
		SRGB_PVRTC_2BPPV1_UNORM,
		SRGB_PVRTC_4BPPV1_UNORM,
		SRGB_ALPHA_PVRTC_2BPPV1_UNORM,
		SRGB_ALPHA_PVRTC_4BPPV1_UNORM,
		SRGB8_ALPHA8_ASTC_4x4,
		SRGB8_ALPHA8_ASTC_5x4,
		SRGB8_ALPHA8_ASTC_5x5,
		SRGB8_ALPHA8_ASTC_6x5,
		SRGB8_ALPHA8_ASTC_6x6,
		SRGB8_ALPHA8_ASTC_8x5,
		SRGB8_ALPHA8_ASTC_8x6,
		SRGB8_ALPHA8_ASTC_8x8,
		SRGB8_ALPHA8_ASTC_10x5,
		SRGB8_ALPHA8_ASTC_10x6,
		SRGB8_ALPHA8_ASTC_10x8,
		SRGB8_ALPHA8_ASTC_10x10,
		SRGB8_ALPHA8_ASTC_12x10,
		SRGB8_ALPHA8_ASTC_12x12, FORMAT_COMPRESSED_LAST = SRGB8_ALPHA8_ASTC_12x12, FORMAT_LAST = SRGB8_ALPHA8_ASTC_12x12
	};

	enum
	{
		FORMAT_COUNT = FORMAT_LAST - FORMAT_FIRST + 1,
		FORMAT_COMPRESSED_COUNT = FORMAT_COMPRESSED_LAST - FORMAT_COMPRESSED_FIRST + 1
	};

	inline bool is_compressed(format const & Format)
	{
		return Format >= FORMAT_COMPRESSED_FIRST && Format <= FORMAT_COMPRESSED_LAST;
	}

	inline bool is_valid(format const & Format)
	{
		return Format >= FORMAT_FIRST && Format <= FORMAT_LAST;
	}

	std::uint32_t block_size(format const & Format);
	std::uint32_t block_dimensions_x(format const & Format);
	std::uint32_t block_dimensions_y(format const & Format);
	std::uint32_t block_dimensions_z(format const & Format);
	std::uint32_t component_count(format const & Format);
}//namespace gli

#include "format.inl"
