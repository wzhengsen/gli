/// @brief Include to compress and decompress s3tc blocks
/// @file gli/s3tc.hpp

#pragma once

namespace gli
{
	namespace detail
	{
		struct dxt1_block {
			uint16_t Color0;
			uint16_t Color1;
			uint8_t Row[4];
		};

		struct dxt3_block {
			uint16_t AlphaRow[4];
			uint16_t Color0;
			uint16_t Color1;
			uint8_t Row[4];
		};

		struct dxt5_block {
			uint8_t Alpha0;
			uint8_t Alpha1;
			uint8_t AlphaR0;
			uint8_t AlphaR1;
			uint8_t AlphaR2;
			uint8_t AlphaR3;
			uint8_t AlphaR4;
			uint8_t AlphaR5;
			uint16_t Color0;
			uint16_t Color1;
			uint8_t Row0;
			uint8_t Row1;
			uint8_t Row2;
			uint8_t Row3;
		};

		struct texel_block4x4 {
			// row x col
			glm::vec4 Texel[4][4];
		};
		
		glm::vec4 decompress_dxt1(const dxt1_block &Block, const extent2d &BlockTexelCoord);
		
		texel_block4x4 decompress_dxt1_block(const dxt1_block &Block);

	}//namespace detail
}//namespace gli

#include "./s3tc.inl"
