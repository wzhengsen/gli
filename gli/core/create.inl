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
/// @file gli/core/create.inl
/// @date 2015-08-09 / 2015-08-09
/// @author Christophe Riccio
///////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "load_ktx.hpp"
#include <algorithm>

namespace gli{
namespace detail
{
	inline target computeTargetKTX(
		std::uint32_t NumberOfArrayElements,
		std::uint32_t NumberOfFaces,
		std::uint32_t PixelHeight,
		std::uint32_t PixelDepth)
	{		
		if(PixelHeight == 0 && PixelDepth == 0 && NumberOfArrayElements == 0 && NumberOfFaces == 0)
			return TARGET_1D;
		else if(PixelHeight == 0 && PixelDepth == 0 && NumberOfArrayElements > 0 && NumberOfFaces == 0)
			return TARGET_1D_ARRAY;		
		else if(PixelHeight > 0 && PixelDepth == 0 && NumberOfArrayElements == 0 && NumberOfFaces == 0)
			return TARGET_2D;
		else if(PixelHeight > 0 && PixelDepth == 0 && NumberOfArrayElements > 0 && NumberOfFaces == 0)
			return TARGET_2D_ARRAY;
		else if(PixelHeight > 0 && PixelDepth > 0 && NumberOfArrayElements == 0 && NumberOfFaces == 0)
			return TARGET_3D;
		else if(PixelHeight > 0 && PixelDepth == 0 && NumberOfArrayElements == 0 && NumberOfFaces > 0)
			return TARGET_CUBE;
		else if(PixelHeight > 0 && PixelDepth == 0 && NumberOfArrayElements > 0 && NumberOfFaces > 0)
			return TARGET_CUBE_ARRAY;
		else
			return TARGET_NONE;
	} 

	inline texture* createKTX(char const * Data, std::size_t Size)
	{
		assert(Data && (Size >= sizeof(detail::ktxHeader)));

		detail::ktxHeader const & Header(*reinterpret_cast<detail::ktxHeader const *>(Data));

		if(strncmp(Header.Identifier, "«KTX 11»\r\n\x1A\n", 12) != 0)
			return nullptr;

		size_t Offset = sizeof(detail::ktxHeader);

		// Skip key value data
		Offset += Header.BytesOfKeyValueData;

		gl GL;
		gli::format const Format = GL.find(
			static_cast<gli::gl::internalFormat>(Header.GLInternalFormat),
			static_cast<gli::gl::externalFormat>(Header.GLFormat),
			static_cast<gli::gl::typeFormat>(Header.GLType));

		assert(Format != static_cast<format>(gli::FORMAT_INVALID));

		gli::target Target = computeTargetKTX(
			Header.NumberOfArrayElements,
			Header.NumberOfFaces,
			Header.PixelHeight,
			Header.PixelDepth);

		texture* Texture = gli::create(
			Target, 
			std::max<std::uint32_t>(Header.NumberOfArrayElements, 1),
			std::max<std::uint32_t>(Header.NumberOfFaces, 1),
			std::max<std::uint32_t>(Header.NumberOfMipmapLevels, 1),
			Format,
			storage::dim_type(
				Header.PixelWidth,
				std::max<std::uint32_t>(Header.PixelHeight, 1),
				std::max<std::uint32_t>(Header.PixelDepth, 1)));
		assert(Texture);

		for(std::size_t Level = 0, Levels = Texture->levels(); Level < Levels; ++Level)
		{
			std::uint32_t const ImageSize = *reinterpret_cast<std::uint32_t const*>(Data + Offset);
			Offset += sizeof(std::uint32_t);

			for(std::size_t Layer = 0, Layers = Texture->layers(); Layer < Layers; ++Layer)
			{
				for(std::size_t Face = 0, Faces = Texture->faces(); Face < Faces; ++Face)
				{
					std::uint32_t const FaceSize = static_cast<std::uint32_t>(Texture->level_size(Level));
					std::uint32_t const DestinationOffset = static_cast<std::uint32_t>(detail::imageAddressing(Texture, Layer, Face, Level));

					std::memcpy(Texture->data() + DestinationOffset, Data + Offset, FaceSize);

					Offset += glm::ceilMultiple(FaceSize, static_cast<std::uint32_t>(4));
				}
			}
		}

		return Texture;
	}
}//namespace detail

	inline texture* create(char const * Path)
	{
		FILE* File = std::fopen(Path, "rb");
		if(!File)
			return nullptr;

		long Beg = std::ftell(File);
		std::fseek(File, 0, SEEK_END);
		long End = std::ftell(File);
		std::fseek(File, 0, SEEK_SET);

		std::vector<char> Data(static_cast<std::size_t>(End - Beg));

		std::fread(&Data[0], 1, Data.size(), File);
		std::fclose(File);

		return create(&Data[0], Data.size());	
	}

	inline texture* create(std::string const & Path)
	{
		return create(Path.c_str());
	}

	inline texture* create(char const * Data, std::size_t Size)
	{
		{
			texture* Texture = detail::createKTX(Data, Size);
			if(Texture)
				return Texture;
		}
/*		{
			texture* Texture = detail::createDDS(Data, Size);
			if(Texture)
				return Texture;
		}*/
		return nullptr;
	}

	inline texture* create(
		target Target,
		size_t const & Layers,
		size_t const & Faces,
		size_t const & Levels,
		format const & Format,
		storage::dim_type const & Dimensions)
	{
		switch(Target)
		{
		case TARGET_1D:
			return new texture1D(Levels, Format, texture1D::dim_type(Dimensions));
		case TARGET_1D_ARRAY:
			return new texture1DArray(Layers, Levels, Format, texture1DArray::dim_type(Dimensions));
		case TARGET_2D:
			return new texture2D(Levels, Format, texture2D::dim_type(Dimensions));
		case TARGET_2D_ARRAY:
			return new texture2DArray(Layers, Levels, Format, texture2DArray::dim_type(Dimensions));
		case TARGET_3D:
			return new texture3D(Levels, Format, texture3D::dim_type(Dimensions));
		case TARGET_CUBE:
			return new textureCube(Faces, Levels, Format, textureCube::dim_type(Dimensions));
		case TARGET_CUBE_ARRAY:
			return new textureCubeArray(Layers, Faces, Levels, Format, textureCubeArray::dim_type(Dimensions));
		default:
			return nullptr;
		}
	}

	inline void release(texture* Texture)
	{
		delete Texture;
		Texture = nullptr;
	}
}//namespace gli
