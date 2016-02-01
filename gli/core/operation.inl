#include <cstring>

namespace gli
{
	namespace detail
	{
		inline image2D duplicate(image2D const & Mipmap2D)
		{
			image2D Result(Mipmap2D.format(), Mipmap2D.dimensions());
			memcpy(Result.data(), Mipmap2D.data(), Mipmap2D.capacity());
			return Result;	
		}

		inline image2D flip(image2D const & Mipmap2D)
		{
			GLI_ASSERT(!Mipmap2D.is_compressed());

			image2D Result(Mipmap2D.format(), Mipmap2D.dimensions());
			
			std::size_t TexelSize = Result.texel_size() >> std::size_t(3);
			glm::byte * DstPtr = reinterpret_cast<glm::byte *>(Result.data());
			glm::byte const * const SrcPtr = reinterpret_cast<glm::byte const * const>(Mipmap2D.data());

			for(std::size_t j = 0; j < Result.dimensions().y; ++j)
			for(std::size_t i = 0; i < Result.dimensions().x; ++i)
			{
				std::size_t DstIndex = (i + j * Result.dimensions().y) * TexelSize;
				std::size_t SrcIndex = (i + (Result.dimensions().y - j) * Result.dimensions().x) * TexelSize;
				memcpy(DstPtr + DstIndex, SrcPtr + SrcIndex, TexelSize);
			}

			return Result;
		}

		inline image2D mirror(image2D const & Mipmap2D)
		{
			image2D Result(Mipmap2D.format(), Mipmap2D.dimensions());

			std::size_t ValueSize = Mipmap2D.texel_size() >> std::size_t(3);
			glm::byte * DstPtr = reinterpret_cast<glm::byte *>(Result.data());
			glm::byte const * const SrcPtr = reinterpret_cast<glm::byte const * const>(Mipmap2D.data());

			for(std::size_t j = 0; j < Result.dimensions().y; ++j)
			for(std::size_t i = 0; i < Result.dimensions().x; ++i)
			{
				std::size_t DstIndex = (i + j * Result.dimensions().x) * ValueSize;
				std::size_t SrcIndex = ((Result.dimensions().x - i) + j * Result.dimensions().x) * ValueSize;
				memcpy(DstPtr + DstIndex, SrcPtr + SrcIndex, ValueSize);
			}

			return Result;
		}

		inline image2D swizzle
		(
			image2D const & Mipmap, 
			glm::uvec4 const & Channel
		)
		{
			image2D Result = gli::detail::duplicate(Mipmap);

			glm::byte * DataDst = reinterpret_cast<glm::byte *>(Result.data());
			glm::byte const * const DataSrc = reinterpret_cast<glm::byte const * const>(Mipmap.data());

			gli::texture2d::size_type CompSize = (Mipmap.texel_size() >> 3) / Mipmap.components();
			gli::texture2d::size_type TexelCount = (Mipmap.capacity() / Mipmap.texel_size()) >> 3;

			for(gli::texture2d::size_type t = 0; t < TexelCount; ++t)
			for(gli::texture2d::size_type c = 0; c < Mipmap.components(); ++c)
			{
				gli::texture2d::size_type IndexSrc = t * Mipmap.components() + Channel[glm::uvec4::size_type(c)];
				gli::texture2d::size_type IndexDst = t * Mipmap.components() + c;

				memcpy(DataDst + IndexDst, DataSrc + IndexSrc, CompSize);
			}

			return Result;
		}

		inline image2D crop
		(
			image2D const & Image, 
			image2D::dimensions_type const & Position, 
			image2D::dimensions_type const & Size
		)
		{
			GLI_ASSERT((Position.x + Size.x) <= Image.dimensions().x && (Position.y + Size.y) <= Image.dimensions().y);

			image2D Result(Image.format(), Size);

			glm::byte* DstData = reinterpret_cast<glm::byte *>(Result.data());
			glm::byte const * const SrcData = reinterpret_cast<glm::byte const * const>(Image.data());

			for(std::size_t j = 0; j < Size.y; ++j)
			{
				std::size_t DstIndex = 0                                + (0         + j) * Size.x               * (Image.texel_size() >> 3);
				std::size_t SrcIndex = Position.x * Image.texel_size() + (Position.y + j) * Image.dimensions().x * (Image.texel_size() >> 3);
				memcpy(DstData + DstIndex, SrcData + SrcIndex, Image.texel_size() * Size.x);	
			}

			return Result;
		}

		inline image2D copy
		(
			image2D const & SrcMipmap, 
			image2D::dimensions_type const & SrcPosition,
			image2D::dimensions_type const & SrcSize,
			image2D & DstMipmap, 
			image2D::dimensions_type const & DstPosition
		)
		{
			GLI_ASSERT((SrcPosition.x + SrcSize.x) <= SrcMipmap.dimensions().x && (SrcPosition.y + SrcSize.y) <= SrcMipmap.dimensions().y);
			GLI_ASSERT(SrcMipmap.format() == DstMipmap.format());

			glm::byte * DstData = reinterpret_cast<glm::byte *>(DstMipmap.data());
			glm::byte const * const SrcData = reinterpret_cast<glm::byte const * const>(SrcMipmap.data());

			std::size_t SizeX = std::min(std::size_t(SrcSize.x + SrcPosition.x), std::size_t(DstMipmap.dimensions().x  + DstPosition.x));
			std::size_t SizeY = std::min(std::size_t(SrcSize.y + SrcPosition.y), std::size_t(DstMipmap.dimensions().y + DstPosition.y));

			for(std::size_t j = 0; j < SizeY; ++j)
			{
				std::size_t DstIndex = DstPosition.x * DstMipmap.texel_size() + (DstPosition.y + j) * DstMipmap.dimensions().x * DstMipmap.texel_size();
				std::size_t SrcIndex = SrcPosition.x * SrcMipmap.texel_size() + (SrcPosition.y + j) * SrcMipmap.dimensions().x * SrcMipmap.texel_size();
				memcpy(DstData + DstIndex, SrcData + SrcIndex, SrcMipmap.texel_size() * SizeX);	
			}

			return DstMipmap;
		}

	}//namespace detail
/*
	inline texture2d duplicate(texture2d const & Texture2D)
	{
		texture2d Result(Texture2D.levels(), Texture2D.format(), Texture2D.dimensions());
		for(texture2d::size_type Level = 0; Level < Texture2D.levels(); ++Level)
			Result[Level] = detail::duplicate(Texture2D[Level]);
		return Result;
	}

	inline texture2d flip(texture2d const & Texture2D)
	{
		texture2d Result(Texture2D.levels(), Texture2D.format(), Texture2D.dimensions());
		for(texture2d::size_type Level = 0; Level < Texture2D.levels(); ++Level)
			Result[Level] = detail::flip(Texture2D[Level]);
		return Result;
	}

	inline texture2d mirror(texture2d const & Texture2D)
	{
		texture2d Result(Texture2D.levels(), Texture2D.format(), Texture2D.dimensions());
		for(texture2d::size_type Level = 0; Level < Texture2D.levels(); ++Level)
			Result[Level] = detail::mirror(Texture2D[Level]);
		return Result;
	}

	inline texture2d crop
	(
		texture2d const & Texture2D,
		texture2d::dimensions_type const & Position,
		texture2d::dimensions_type const & Size
	)
	{
		texture2d Result(Texture2D.levels(), Texture2D.format(), Texture2D.dimensions());
		for(texture2d::size_type Level = 0; Level < Texture2D.levels(); ++Level)
			Result[Level] = detail::crop(
				Texture2D[Level], 
				Position >> texture2d::dimensions_type(Level), 
				Size >> texture2d::dimensions_type(Level));
		return Result;
	}

	inline texture2d swizzle
	(
		texture2d const & Texture2D,
		glm::uvec4 const & Channel
	)
	{
		texture2d Result(Texture2D.levels(), Texture2D.format(), Texture2D.dimensions());
		for(texture2d::size_type Level = 0; Level < Texture2D.levels(); ++Level)
			Result[Level] = detail::swizzle(Texture2D[Level], Channel);
		return Result;
	}

	inline texture2d copy
	(
		texture2d const & SrcImage, 
		texture2d::size_type const & SrcLevel,
		texture2d::dimensions_type const & SrcPosition,
		texture2d::dimensions_type const & SrcDimensions,
		texture2d & DstMipmap, 
		texture2d::size_type const & DstLevel,
		texture2d::dimensions_type const & DstDimensions
	)
	{
		detail::copy(
			SrcImage[SrcLevel], 
			SrcPosition, 
			SrcDimensions,
			DstMipmap[DstLevel],
			DstDimensions);
		return DstMipmap;
	}

	//inline image operator+(image const & MipmapA, image const & MipmapB)
	//{
	//	
	//}

	//inline image operator-(image const & MipmapA, image const & MipmapB)
	//{
	//	
	//}

	//inline image operator*(image const & MipmapA, image const & MipmapB)
	//{
	//	
	//}

	//inline image operator/(image const & MipmapA, image const & MipmapB)
	//{
	//	
	//}
*/
}//namespace gli
