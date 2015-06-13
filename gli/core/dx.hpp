///////////////////////////////////////////////////////////////////////////////////
/// OpenGL Image (gli.g-truc.net)
///
/// Copyright (c) 2008 - 2013 G-Truc Creation (www.g-truc.net)
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
/// @file gli/core/gl.hpp
/// @date 2013-11-09 / 2013-11-09
/// @author Christophe Riccio
///////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "storage.hpp"
#include <array>

namespace gli
{
	class dx
	{
	public:
		#define GLI_MAKEFOURCC(ch0, ch1, ch2, ch3) \
			(std::uint32_t)( \
			(((std::uint32_t)(std::uint8_t)(ch3) << 24) & 0xFF000000) | \
			(((std::uint32_t)(std::uint8_t)(ch2) << 16) & 0x00FF0000) | \
			(((std::uint32_t)(std::uint8_t)(ch1) <<  8) & 0x0000FF00) | \
			((std::uint32_t)(std::uint8_t)(ch0)        & 0x000000FF) )

		enum D3DFORMAT
		{
			D3DFMT_UNKNOWN              =  0,

			D3DFMT_R8G8B8               = 20,
			D3DFMT_A8R8G8B8             = 21,
			D3DFMT_X8R8G8B8             = 22,
			D3DFMT_R5G6B5               = 23,
			D3DFMT_X1R5G5B5             = 24,
			D3DFMT_A1R5G5B5             = 25,
			D3DFMT_A4R4G4B4             = 26,
			D3DFMT_R3G3B2               = 27,
			D3DFMT_A8                   = 28,
			D3DFMT_A8R3G3B2             = 29,
			D3DFMT_X4R4G4B4             = 30,
			D3DFMT_A2B10G10R10          = 31,
			D3DFMT_A8B8G8R8             = 32,
			D3DFMT_X8B8G8R8             = 33,
			D3DFMT_G16R16               = 34,
			D3DFMT_A2R10G10B10          = 35,
			D3DFMT_A16B16G16R16         = 36,

			D3DFMT_A8P8                 = 40,
			D3DFMT_P8                   = 41,

			D3DFMT_L8                   = 50,
			D3DFMT_A8L8                 = 51,
			D3DFMT_A4L4                 = 52,

			D3DFMT_V8U8                 = 60,
			D3DFMT_L6V5U5               = 61,
			D3DFMT_X8L8V8U8             = 62,
			D3DFMT_Q8W8V8U8             = 63,
			D3DFMT_V16U16               = 64,
			D3DFMT_A2W10V10U10          = 67,

			D3DFMT_UYVY                 = GLI_MAKEFOURCC('U', 'Y', 'V', 'Y'),
			D3DFMT_R8G8_B8G8            = GLI_MAKEFOURCC('R', 'G', 'B', 'G'),
			D3DFMT_YUY2                 = GLI_MAKEFOURCC('Y', 'U', 'Y', '2'),
			D3DFMT_G8R8_G8B8            = GLI_MAKEFOURCC('G', 'R', 'G', 'B'),
			D3DFMT_DXT1                 = GLI_MAKEFOURCC('D', 'X', 'T', '1'),
			D3DFMT_DXT2                 = GLI_MAKEFOURCC('D', 'X', 'T', '2'),
			D3DFMT_DXT3                 = GLI_MAKEFOURCC('D', 'X', 'T', '3'),
			D3DFMT_DXT4                 = GLI_MAKEFOURCC('D', 'X', 'T', '4'),
			D3DFMT_DXT5                 = GLI_MAKEFOURCC('D', 'X', 'T', '5'),

			D3DFMT_ATI1                 = GLI_MAKEFOURCC('A', 'T', 'I', '1'),
			D3DFMT_AT1N                 = GLI_MAKEFOURCC('A', 'T', '1', 'N'),
			D3DFMT_ATI2                 = GLI_MAKEFOURCC('A', 'T', 'I', '2'),
			D3DFMT_AT2N                 = GLI_MAKEFOURCC('A', 'T', '2', 'N'),

			D3DFMT_ETC1                 = GLI_MAKEFOURCC('E', 'T', 'C', '1'),

			D3DFMT_POWERVR_2BPP         = GLI_MAKEFOURCC('P', 'T', 'C', '2'),
			D3DFMT_POWERVR_4BPP         = GLI_MAKEFOURCC('P', 'T', 'C', '4'),

			D3DFMT_D16_LOCKABLE         = 70,
			D3DFMT_D32                  = 71,
			D3DFMT_D15S1                = 73,
			D3DFMT_D24S8                = 75,
			D3DFMT_D24X8                = 77,
			D3DFMT_D24X4S4              = 79,
			D3DFMT_D16                  = 80,

			D3DFMT_D32F_LOCKABLE        = 82,
			D3DFMT_D24FS8               = 83,

			D3DFMT_L16                  = 81,

			D3DFMT_VERTEXDATA           =100,
			D3DFMT_INDEX16              =101,
			D3DFMT_INDEX32              =102,

			D3DFMT_Q16W16V16U16         =110,

			D3DFMT_MULTI2_ARGB8         = GLI_MAKEFOURCC('M','E','T','1'),

			D3DFMT_R16F                 = 111,
			D3DFMT_G16R16F              = 112,
			D3DFMT_A16B16G16R16F        = 113,

			D3DFMT_R32F                 = 114,
			D3DFMT_G32R32F              = 115,
			D3DFMT_A32B32G32R32F        = 116,

			D3DFMT_CxV8U8               = 117,

			D3DFMT_DX10                 = GLI_MAKEFOURCC('D', 'X', '1', '0'),

			D3DFMT_FORCE_DWORD          = 0x7fffffff
		};

		enum DDPF
		{
			DDPF_ALPHAPIXELS = 0x1,
			DDPF_ALPHA = 0x2,
			DDPF_FOURCC = 0x4,
			DDPF_RGB = 0x40,
			DDPF_YUV = 0x200,
			DDPF_LUMINANCE = 0x20000,
			DDPF_FOURCC_ALPHAPIXELS = DDPF_FOURCC | DDPF_ALPHAPIXELS,
			DDPF_RGBAPIXELS = DDPF_RGB | DDPF_ALPHAPIXELS,
			DDPF_RGBA = DDPF_RGB | DDPF_ALPHA,
			DDPF_LUMINANCE_ALPHAPIXELS = DDPF_LUMINANCE | DDPF_ALPHAPIXELS,

		};

		struct format
		{
			DDPF DDPixelFormat;
			D3DFORMAT D3DFormat;
			dxgiFormat DXGIFormat;
			glm::u32vec4 Mask;
		};

	public:
		dx();

		format const & translate(gli::format const & Format) const;

	private:
		std::array<format, FORMAT_COUNT> Translation;
	};
}//namespace gli

#include "dx.inl"
