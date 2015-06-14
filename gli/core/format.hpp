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
		R8_SRGB,
		RG8_SRGB,
		RGB8_SRGB,
		RGBA8_SRGB,

		// Packed formats
		RGB9E5_UFLOAT,
		RG11B10_UFLOAT,
		R3G3B2_UNORM,
		R5G6B5_UNORM,
		RGB5A1_UNORM,
		RGBA4_UNORM,

		// Swizzle formats
		BGR8_UNORM,
		BGRA8_UNORM,
		BGR8_SRGB,
		BGRA8_SRGB,

		// Luminance Alpha formats
		L8_UNORM,
		A8_UNORM,
		LA8_UNORM,
		L16_UNORM,
		A16_UNORM,
		LA16_UNORM,

		// Depth formats
		D16_UNORM,
		D24_UNORM,
		D24S8_UNORM,
		D32_UFLOAT,
		D32_UFLOAT_S8_UNORM,

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
		RGB_PVRTC_4BPP_UNORM,
		RGB_PVRTC_2BPP_UNORM,
		RGBA_PVRTC_4BPP_UNORM,
		RGBA_PVRTC_2BPP_UNORM,
		RGBA_PVRTC2_4BPP_UNORM,
		RGBA_PVRTC2_2BPP_UNORM,
		RGB_ATC_UNORM,
		RGBA_ATC_EXPLICIT_UNORM,
		RGBA_ATC_INTERPOLATED_UNORM,
		RGB_ETC2_UNORM,
		RGBA_ETC2_PUNCHTHROUGH_UNORM,
		RGBA_ETC2_UNORM,
		R11_EAC_UNORM,
		R11_EAC_SNORM,
		RG11_EAC_UNORM,
		RG11_EAC_SNORM,
		RGBA_ASTC4X4_UNORM,
		RGBA_ASTC5X4_UNORM,
		RGBA_ASTC5X5_UNORM,
		RGBA_ASTC6X5_UNORM,
		RGBA_ASTC6X6_UNORM,
		RGBA_ASTC8X5_UNORM,
		RGBA_ASTC8X6_UNORM,
		RGBA_ASTC8X8_UNORM,
		RGBA_ASTC10X5_UNORM,
		RGBA_ASTC10X6_UNORM,
		RGBA_ASTC10X8_UNORM,
		RGBA_ASTC10X10_UNORM,
		RGBA_ASTC12X10_UNORM,
		RGBA_ASTC12X12_UNORM,

		// Compressed sRGB formats
		RGB_DXT1_SRGB,
		RGB_ALPHA_DXT1_SRGB,
		RGB_ALPHA_DXT3_SRGB,
		RGB_ALPHA_DXT5_SRGB,
		RGB_BP_SRGB,
		RGB_PVRTC_2BPPV1_SRGB,
		RGB_PVRTC_4BPPV1_SRGB,
		RGBA_PVRTC_2BPP_SRGB,
		RGBA_PVRTC_4BPP_SRGB,
		RGBA_PVRTC2_2BPP_SRGB,
		RGBA_PVRTC2_4BPP_SRGB,
		RGB_ETC2_SRGB,
		RGBA_ETC2_PUNCHTHROUGH_SRGB,
		RGBA_ETC2_SRGB,
		RGBA_ASTC4X4_SRGB,
		RGBA_ASTC5X4_SRGB,
		RGBA_ASTC5X5_SRGB,
		RGBA_ASTC6X5_SRGB,
		RGBA_ASTC6X6_SRGB,
		RGBA_ASTC8X5_SRGB,
		RGBA_ASTC8X6_SRGB,
		RGBA_ASTC8X8_SRGB,
		RGBA_ASTC10X5_SRGB,
		RGBA_ASTC10X6_SRGB,
		RGBA_ASTC10X8_SRGB,
		RGBA_ASTC10X10_SRGB,
		RGBA_ASTC12X10_SRGB,
		RGBA_ASTC12X12_SRGB, FORMAT_COMPRESSED_LAST = RGBA_ASTC12X12_SRGB, FORMAT_LAST = RGBA_ASTC12X12_SRGB
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
