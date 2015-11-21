//////////////////////////////////////////////////////////////////////////////////
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
/// @file gli/test/core/core_load.cpp
/// @date 2013-11-25 / 2015-08-08
/// @author Christophe Riccio
///////////////////////////////////////////////////////////////////////////////////

#include <gli/gli.hpp>
#include <glm/gtc/epsilon.hpp>
#include <glm/gtc/vec1.hpp>
#include <glm/gtc/packing.hpp>
#include <glm/gtc/color_space.hpp>
#include <ctime>

namespace
{
	std::string path(std::string const & filename, char const * ext)
	{
		return std::string(SOURCE_DIR) + "/data/" + filename + ext;
	}

	struct params
	{
		params(std::string const & Filename, gli::format Format)
			: Filename(Filename)
			, Format(Format)
		{}

		std::string Filename;
		gli::format Format;
	};
}//namespace

namespace load_file_ktx
{
	int test(params const & Params)
	{
		int Error(0);

		gli::texture TextureKTX(gli::load(path(Params.Filename, ".ktx")));
		Error += TextureKTX.format() == Params.Format ? 0 : 1;

		gli::save(TextureKTX, Params.Filename + ".dds");
		gli::texture TextureSavedDDS(gli::load(Params.Filename + ".dds"));
		Error += TextureSavedDDS.format() == Params.Format ? 0 : 1;
		Error += TextureSavedDDS == TextureKTX ? 0 : 1;

		gli::save(TextureKTX, Params.Filename + ".ktx");
		gli::texture TextureSavedKTX(gli::load(Params.Filename + ".ktx"));
		Error += TextureSavedKTX.format() == Params.Format ? 0 : 1;
		Error += TextureSavedDDS == TextureSavedKTX ? 0 : 1;

		return Error;
	}
}//namespace load_file_ktx

namespace load_file_kmg
{
	int test(params const & Params)
	{
		int Error(0);

		gli::texture TextureKTX(gli::load(path(Params.Filename, ".ktx")));
		Error += TextureKTX.format() == Params.Format ? 0 : 1;

		gli::save(TextureKTX, Params.Filename + ".kmg");
		gli::texture TextureSavedKMG(gli::load(Params.Filename + ".kmg"));
		Error += TextureSavedKMG.format() == Params.Format ? 0 : 1;
		Error += TextureSavedKMG == TextureKTX ? 0 : 1;

		gli::save(TextureKTX, Params.Filename + ".ktx");
		gli::texture TextureSavedKTX(gli::load(Params.Filename + ".ktx"));
		Error += TextureSavedKTX.format() == Params.Format ? 0 : 1;
		Error += TextureSavedKTX == TextureKTX ? 0 : 1;

		return Error;
	}
}//namespace load_file_kmg

namespace load_file_dds
{
	int test(params const & Params)
	{
		int Error(0);

		gli::texture TextureDDS(gli::load(path(Params.Filename, ".dds")));
		Error += TextureDDS.format() == Params.Format ? 0 : 1;

		gli::save(TextureDDS, Params.Filename + ".kmg");
		gli::texture TextureSavedKMG(gli::load(Params.Filename + ".kmg"));

		Error += TextureSavedKMG.format() == Params.Format ? 0 : 1;
		Error += TextureSavedKMG == TextureDDS ? 0 : 1;

		gli::save(TextureDDS, Params.Filename + ".dds");
		gli::texture TextureSavedDDS(gli::load(Params.Filename + ".dds"));

		Error += TextureSavedDDS.format() == Params.Format ? 0 : 1;
		Error += TextureSavedDDS == TextureDDS ? 0 : 1;

		return Error;
	}
}//namespace load_file_dds

namespace gen
{
	int test()
	{
		int Error = 0;
		
		for(int TargetIndex = gli::TARGET_FIRST, TargetCount = gli::TARGET_LAST; TargetIndex < TargetCount; ++TargetIndex)
		for(int FormatIndex = gli::FORMAT_FIRST, FormatCount = gli::FORMAT_LAST; FormatIndex < FormatCount; ++FormatIndex)
		{
			gli::target Target = static_cast<gli::target>(TargetIndex);
			gli::format Format = static_cast<gli::format>(FormatIndex);
			
			if((gli::is_compressed(Format) && (gli::is_target_1d(Target) || Target == gli::TARGET_3D)) || gli::is_target_rect(Target))
				continue;
			
			gli::size_t const Layers = gli::is_target_array(Target) ? 2 : 1;
			gli::size_t const Faces = gli::is_target_cube(Target) ? 6 : 1;
			gli::ivec3 const BlockDimensions = gli::block_dimensions(Format);
			
			gli::texture Texture(Target, Format, BlockDimensions * gli::ivec3(BlockDimensions.y, BlockDimensions.x, 1), Layers, Faces, 2);
			Texture.clear();
			
			gli::save(Texture, "gen_test.dds");
			gli::texture TextureDDS(gli::load("gen_test.dds"));
			Error += Texture == TextureDDS ? 0 : 1;
			
			gli::save(Texture, "gen_test.ktx");
			gli::texture TextureKTX(gli::load("gen_test.ktx"));
			Error += Texture == TextureKTX ? 0 : 1;

			gli::save(Texture, "gen_test.kmg");
			gli::texture TextureKMG(gli::load("gen_test.kmg"));
			Error += Texture == TextureKMG ? 0 : 1;
			
			assert(!Error);
		}
		
		return Error;
	}
}//namespace gen

namespace gen_rect
{
	int test()
	{
		int Error = 0;
		
		for(int TargetIndex = gli::TARGET_RECT, TargetCount = gli::TARGET_RECT_ARRAY; TargetIndex <= TargetCount; ++TargetIndex)
		for(int FormatIndex = gli::FORMAT_FIRST, FormatCount = gli::FORMAT_LAST; FormatIndex < FormatCount; ++FormatIndex)
		{
			gli::target Target = static_cast<gli::target>(TargetIndex);
			gli::format Format = static_cast<gli::format>(FormatIndex);
				
			if(gli::is_compressed(Format) && (Target == gli::TARGET_3D))
				continue;
				
			gli::size_t const Layers = gli::is_target_array(Target) ? 2 : 1;
			gli::size_t const Faces = gli::is_target_cube(Target) ? 6 : 1;
			gli::ivec3 const BlockDimensions = gli::block_dimensions(Format);
				
			gli::texture Texture(Target, Format, BlockDimensions * gli::ivec3(BlockDimensions.y, BlockDimensions.x, 1), Layers, Faces, 2);
			Texture.clear();
				
			gli::save(Texture, "gen_rect_test.kmg");
			gli::texture TextureKMG(gli::load("gen_rect_test.kmg"));
			Error += Texture == TextureKMG ? 0 : 1;
				
			assert(!Error);
		}
		
		return Error;
	}
}//namespace gen_rect

namespace make_file_ktx
{
	int test(params const & Params)
	{
		int Error(0);

		gli::texture TextureDDS(gli::load(path(Params.Filename, ".ktx")));
		Error += TextureDDS.format() == Params.Format ? 0 : 1;

		gli::save(TextureDDS, Params.Filename + ".dds");

		return Error;
	}
}//namespace make_file_ktx

int main()
{
	//make_file_ktx::test(params("kueken7_rgba_astc4x4_srgb", gli::FORMAT_RGB8_UNORM_PACK8));

	int Error(0);
	
	Error += gen::test();
	Error += gen_rect::test();
	
	std::vector<params> Params;
	Params.push_back(params("array_r8_uint", gli::FORMAT_R8_UINT_PACK8));
	Params.push_back(params("kueken7_rgba8_unorm", gli::FORMAT_RGBA8_UNORM_PACK8));
	Params.push_back(params("kueken7_rgba8_srgb", gli::FORMAT_RGBA8_SRGB_PACK8));
	Params.push_back(params("kueken7_bgra8_unorm", gli::FORMAT_BGRA8_UNORM_PACK8));
	Params.push_back(params("kueken7_bgra8_srgb", gli::FORMAT_BGRA8_SRGB_PACK8));
	Params.push_back(params("kueken7_r5g6b5_unorm", gli::FORMAT_B5G6R5_UNORM_PACK16));
	Params.push_back(params("kueken7_rgba4_unorm", gli::FORMAT_BGRA4_UNORM_PACK16));
	Params.push_back(params("kueken7_rgb5a1_unorm", gli::FORMAT_BGR5A1_UNORM_PACK16));
	Params.push_back(params("kueken8_rgba8_srgb", gli::FORMAT_RGBA8_SRGB_PACK8));
	Params.push_back(params("kueken7_rgba_dxt5_unorm", gli::FORMAT_RGBA_DXT5_UNORM_BLOCK16));

	{
		for(std::size_t Index = 0, Count = Params.size(); Index < Count; ++Index)
		{
			Error += load_file_ktx::test(Params[Index]);
			Error += load_file_kmg::test(Params[Index]);
			Error += load_file_dds::test(Params[Index]);
		}
	}
	
	return Error;
}
