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
/// @file gli/core/save_ktx.inl
/// @date 2015-08-05 / 2015-08-05
/// @author Christophe Riccio
///////////////////////////////////////////////////////////////////////////////////

#include <cstdio>
#include <glm/gtc/round.hpp>

namespace gli
{
	inline void save_ktx(storage const & Storage, std::vector<char> & Memory)
	{
		if(Storage.empty())
			return;

		gl GL;
		gl::format const & Format = GL.translate(Storage.format());

		detail::format_info const & Desc = detail::getFormatInfo(Storage.format());

		Memory.resize(Storage.size() + sizeof(detail::ktxHeader));

		detail::ktxHeader & Header = *reinterpret_cast<detail::ktxHeader*>(&Memory[0]);

		memcpy(Header.Identifier, "«KTX 11»\r\n\x1A\n", sizeof(Header.Identifier));
		Header.Endianness = 0x04030201;
		Header.GLType = Format.Type;
		Header.GLTypeSize = Format.Type == gl::TYPE_NONE ? 1 : Desc.BlockSize;
		Header.GLFormat = Format.External;
		Header.GLInternalFormat = Format.Internal;
		Header.GLBaseInternalFormat = Format.External;
		Header.PixelWidth = static_cast<std::uint32_t>(Storage.dimensions(0).x);
		Header.PixelHeight = static_cast<std::uint32_t>(Storage.dimensions(0).y);
		Header.PixelDepth = static_cast<std::uint32_t>(Storage.dimensions(0).z);
		Header.NumberOfArrayElements = static_cast<std::uint32_t>(Storage.layers());
		Header.NumberOfFaces = static_cast<std::uint32_t>(Storage.faces());
		Header.NumberOfMipmapLevels = static_cast<std::uint32_t>(Storage.levels());
		Header.BytesOfKeyValueData = 0;

		std::size_t Offset = sizeof(detail::ktxHeader);

		for(std::size_t Level = 0, Levels = Storage.levels(); Level < Levels; ++Level)
		{
			std::uint32_t& ImageSize = *reinterpret_cast<std::uint32_t*>(&Memory[0] + Offset);
			Offset += sizeof(std::uint32_t);

			for(std::size_t Layer = 0, Layers = Storage.layers(); Layer < Layers; ++Layer)
			{
				for(std::size_t Face = 0, Faces = Storage.faces(); Face < Faces; ++Face)
				{
					std::uint32_t const FaceSize = static_cast<std::uint32_t>(Storage.level_size(Level));
					std::uint32_t const SourceOffset = static_cast<std::uint32_t>(detail::imageAddressing(Storage, Layer, Face, Level));

					std::memcpy(&Memory[0] + Offset, Storage.data() + SourceOffset, FaceSize);

					std::uint32_t const PaddedSize = glm::ceilMultiple(FaceSize, static_cast<std::uint32_t>(4));

					ImageSize += PaddedSize;
					Offset += PaddedSize;
				}
			}

			ImageSize = glm::ceilMultiple(ImageSize, static_cast<std::uint32_t>(4));
		}
	}

	inline void save_ktx(storage const & Storage, char const * Filename)
	{
		if(Storage.empty())
			return;

		FILE* File = std::fopen(Filename, "wb");
		if (!File)
			return;

		std::vector<char> Memory;
		save_ktx(Storage, Memory);

		std::fwrite(&Memory[0], 1, Memory.size(), File);
		std::fclose(File);
	}

	inline void save_ktx(storage const & Storage, std::string const & Filename)
	{
		save_ktx(Storage, Filename.c_str());
	}
}//namespace gli
