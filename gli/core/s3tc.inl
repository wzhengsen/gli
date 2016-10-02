#include <glm/gtc/packing.hpp>

namespace gli
{
	namespace detail
	{
		inline glm::vec4 decompress_dxt1(const dxt1_block &Block, const extent2d &BlockTexelCoord)
		{
			glm::vec4 Color[4];

			Color[0] = glm::vec4(unpackUnorm1x5_1x6_1x5(Block.Color0), 1.0f);
			std::swap(Color[0].r, Color[0].b);
			Color[1] = glm::vec4(unpackUnorm1x5_1x6_1x5(Block.Color1), 1.0f);
			std::swap(Color[1].r, Color[1].b);

			if(Block.Color0 > Block.Color1)
			{
				Color[2] = (2.0f / 3.0f) * Color[0] + (1.0f / 3.0f) * Color[1];
				Color[3] = (1.0f / 3.0f) * Color[0] + (2.0f / 3.0f) * Color[1];
			}
			else
			{
				Color[2] = (Color[0] + Color[1]) / 2.0f;
				Color[3] = glm::vec4(0.0f);
			}

			glm::uint8 ColorIndex = (Block.Row[BlockTexelCoord.y] >> (BlockTexelCoord.x * 2)) & 0x3;
			return Color[ColorIndex];
		}

		inline texel_block4x4 decompress_dxt1_block(const dxt1_block &Block)
		{
			glm::vec4 Color[4];

			Color[0] = glm::vec4(unpackUnorm1x5_1x6_1x5(Block.Color0), 1.0f);
			std::swap(Color[0].r, Color[0].b);
			Color[1] = glm::vec4(unpackUnorm1x5_1x6_1x5(Block.Color1), 1.0f);
			std::swap(Color[1].r, Color[1].b);

			if(Block.Color0 > Block.Color1)
			{
				Color[2] = (2.0f / 3.0f) * Color[0] + (1.0f / 3.0f) * Color[1];
				Color[3] = (1.0f / 3.0f) * Color[0] + (2.0f / 3.0f) * Color[1];
			}
			else
			{
				Color[2] = (Color[0] + Color[1]) / 2.0f;
				Color[3] = glm::vec4(0.0f);
			}

			texel_block4x4 TexelBlock;
			for(glm::uint8 Row = 0; Row < 4; ++Row)
			{
				for(glm::uint8 Col = 0; Col < 4; ++Col)
				{
					glm::uint8 ColorIndex = (Block.Row[Row] >> (Col * 2)) & 0x3;
					TexelBlock.Texel[Row][Col] = Color[ColorIndex];
				}
			}
			
			return TexelBlock;
		}
	}//namespace detail
}//namespace gli
