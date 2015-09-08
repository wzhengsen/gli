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
/// @file gli/core/load_kim.inl
/// @date 2015-09-08 / 2015-09-08
/// @author Christophe Riccio
///////////////////////////////////////////////////////////////////////////////////

#include <cstdio>
#include <cassert>

namespace gli{
namespace detail
{
	static unsigned char const FOURCC_KIM10[] = {0xAB, 0x4B, 0x49, 0x4D, 0x20, 0x31, 0x31, 0xBB, 0x0D, 0x0A, 0x1A, 0x0A};

	struct kimHeader10
	{
		std::uint32_t Endianness;
		std::uint32_t Format;
		std::uint32_t Target;
		std::uint32_t SwizzleRed;
		std::uint32_t SwizzleGreen;
		std::uint32_t SwizzleBlue;
		std::uint32_t SwizzleAlpha;
		std::uint32_t PixelWidth;
		std::uint32_t PixelHeight;
		std::uint32_t PixelDepth;
		std::uint32_t Layers;
		std::uint32_t Levels;
		std::uint32_t Faces;
		std::uint32_t GenerateMipmaps;
	};

	inline texture load_kim10(char const * Data, std::size_t Size)
	{
		detail::kimHeader10 const & Header(*reinterpret_cast<detail::kimHeader10 const *>(Data));

		size_t Offset = sizeof(detail::kimHeader10);

		texture Texture(
			static_cast<target>(Header.Target),
			static_cast<format>(Header.Format),
			texture::dim_type(Header.PixelWidth, Header.PixelHeight, Header.PixelDepth),
			Header.Layers,
			Header.Levels,
			Header.Faces,
			texture::swizzles_type(Header.SwizzleRed, Header.SwizzleGreen, Header.SwizzleBlue, Header.SwizzleAlpha));

		for(std::size_t Level = 0, Levels = Texture.levels(); Level < Levels; ++Level)
		{
			Offset += sizeof(std::uint32_t);

			for(std::size_t Layer = 0, Layers = Texture.layers(); Layer < Layers; ++Layer)
			{
				for(std::size_t Face = 0, Faces = Texture.faces(); Face < Faces; ++Face)
				{
					std::uint32_t const FaceSize = static_cast<std::uint32_t>(Texture.size(Level));

					std::memcpy(Texture.data(Layer, Face, Level), Data + Offset, FaceSize);

					Offset += FaceSize;
				}
			}
		}

		return Texture;
	}
}//namespace detail

	inline texture load_kim(char const * Data, std::size_t Size)
	{
		assert(Data && (Size >= sizeof(detail::kimHeader10)));

		// KIM10
		{
			if(memcmp(Data, detail::FOURCC_KIM10, sizeof(detail::FOURCC_KIM10)) == 0)
				return detail::load_kim10(Data + sizeof(detail::FOURCC_KIM10), Size - sizeof(detail::FOURCC_KIM10));
		}

		return texture();
	}

	inline texture load_kim(char const * Filename)
	{
		FILE* File = std::fopen(Filename, "rb");
		if(!File)
			return texture();

		long Beg = std::ftell(File);
		std::fseek(File, 0, SEEK_END);
		long End = std::ftell(File);
		std::fseek(File, 0, SEEK_SET);

		std::vector<char> Data(static_cast<std::size_t>(End - Beg));

		std::fread(&Data[0], 1, Data.size(), File);
		std::fclose(File);

		return load_kim(&Data[0], Data.size());
	}

	inline texture load_kim(std::string const & Filename)
	{
		return load_kim(Filename.c_str());
	}
}//namespace gli
