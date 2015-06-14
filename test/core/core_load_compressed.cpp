//////////////////////////////////////////////////////////////////////////////////
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
/// @file gli/core/fetch.cpp
/// @date 2013-11-25 / 2013-11-25
/// @author Christophe Riccio
///////////////////////////////////////////////////////////////////////////////////

#include <gli/gli.hpp>
#include <glm/gtc/epsilon.hpp>
#include <glm/gtc/vec1.hpp>
#include <glm/gtc/packing.hpp>
#include <glm/gtc/color.hpp>

namespace load_ati1n
{
	int test()
	{
		int Error(0);

		gli::texture2D TextureA(gli::load_dds("kueken1_ati1n.dds"));
		gli::save_dds(TextureA, "kueken1_ati1n_copy.dds");
		gli::texture2D TextureB(gli::load_dds("kueken1_ati1n_copy.dds"));

		Error += TextureA == TextureB ? 0 : 1;

		return Error;
	}
}//namespace load_ati1n

namespace load_ati2n
{
	int test()
	{
		int Error(0);

		gli::texture2D TextureA(gli::load_dds("kueken1_ati2n.dds"));
		gli::save_dds(TextureA, "kueken1_ati2n_copy.dds");
		gli::texture2D TextureB(gli::load_dds("kueken1_ati2n_copy.dds"));

		Error += TextureA == TextureB ? 0 : 1;

		return Error;
	}
}//namespace load_ati2n

namespace load_bgr8
{
	int test()
	{
		int Error(0);

		gli::texture2D TextureA(gli::load_dds("kueken1_bgr8.dds"));
		gli::save_dds(TextureA, "kueken1_bgr8_copy.dds");
		gli::texture2D TextureB(gli::load_dds("kueken1_bgr8_copy.dds"));

		Error += TextureA == TextureB ? 0 : 1;

		return Error;
	}
}//namespace load_bgr8

namespace load_dxt1
{
	int test()
	{
		int Error(0);

		gli::texture2D TextureA(gli::load_dds("kueken1_dxt1.dds"));
		gli::save_dds(TextureA, "kueken1_dxt1_copy.dds");
		gli::texture2D TextureB(gli::load_dds("kueken1_dxt1_copy.dds"));

		Error += TextureA == TextureB ? 0 : 1;

		return Error;
	}
}//namespace load_dxt1

namespace load_dxt5
{
	int test()
	{
		int Error(0);

		gli::texture2D TextureA(gli::load_dds("kueken1_dxt5.dds"));
		gli::save_dds(TextureA, "kueken1_dxt5_copy.dds");
		gli::texture2D TextureB(gli::load_dds("kueken1_dxt5_copy.dds"));

		Error += TextureA == TextureB ? 0 : 1;

		return Error;
	}
}//namespace load_dxt5

namespace load_bc4
{
	int test()
	{
		int Error(0);

		gli::texture2D TextureA(gli::load_dds("kueken2_bc4.dds"));
		gli::save_dds(TextureA, "kueken2_bc4_copy.dds");
		gli::texture2D TextureB(gli::load_dds("kueken2_bc4_copy.dds"));

		Error += TextureA == TextureB ? 0 : 1;

		return Error;
	}
}//namespace load_bc4

namespace load_bc5
{
	int test()
	{
		int Error(0);

		gli::texture2D TextureA(gli::load_dds("kueken2_bc5.dds"));
		gli::save_dds(TextureA, "kueken2_bc5_copy.dds");
		gli::texture2D TextureB(gli::load_dds("kueken2_bc5_copy.dds"));

		Error += TextureA == TextureB ? 0 : 1;

		return Error;
	}
}//namespace load_bc5

namespace load_bc7
{
	int test()
	{
		int Error(0);

		gli::texture2D TextureA(gli::load_dds("kueken2_bc7.dds"));
		gli::save_dds(TextureA, "kueken2_bc7_copy.dds");
		gli::texture2D TextureB(gli::load_dds("kueken2_bc7_copy.dds"));

		Error += TextureA == TextureB ? 0 : 1;

		return Error;
	}
}//namespace load_bc7

namespace load_bgra8
{
	int test()
	{
		int Error(0);

		gli::texture2D TextureA(gli::load_dds("kueken2_bgra8.dds"));
		gli::save_dds(TextureA, "kueken2_bgra8_copy.dds");
		gli::texture2D TextureB(gli::load_dds("kueken2_bgra8_copy.dds"));

		Error += TextureA == TextureB ? 0 : 1;

		return Error;
	}
}//namespace load_bgra8

namespace load_bgr8b
{
	int test()
	{
		int Error(0);

		gli::texture2D TextureA(gli::load_dds("kueken3_bgr8.dds"));
		gli::save_dds(TextureA, "kueken3_bgr8_copy.dds");
		gli::texture2D TextureB(gli::load_dds("kueken3_bgr8_copy.dds"));

		Error += TextureA == TextureB ? 0 : 1;

		return Error;
	}
}//namespace load_bgr8b

namespace load_bc1_rgb
{
	int test()
	{
		int Error(0);

		gli::texture2D TextureA(gli::load_dds("kueken7_bc1_rgb.dds"));
		gli::save_dds(TextureA, "kueken7_bc1_rgb_copy.dds");
		gli::texture2D TextureB(gli::load_dds("kueken7_bc1_rgb_copy.dds"));

		Error += TextureA == TextureB ? 0 : 1;

		return Error;
	}
}//namespace load_bc1_rgb

namespace load_bc1_srgb
{
	int test()
	{
		int Error(0);

		gli::texture2D TextureA(gli::load_dds("kueken7_bc1_srgb.dds"));
		gli::save_dds(TextureA, "kueken7_bc1_srgb_copy.dds");
		gli::texture2D TextureB(gli::load_dds("kueken7_bc1_srgb_copy.dds"));

		Error += TextureA == TextureB ? 0 : 1;

		return Error;
	}
}//namespace load_bc1_srgb

namespace load_bc3_rgba
{
	int test()
	{
		int Error(0);

		gli::texture2D TextureA(gli::load_dds("kueken7_bc3_rgba.dds"));
		gli::save_dds(TextureA, "kueken7_bc3_rgba_copy.dds");
		gli::texture2D TextureB(gli::load_dds("kueken7_bc3_rgba_copy.dds"));

		Error += TextureA == TextureB ? 0 : 1;

		return Error;
	}
}//namespace load_bc3_rgba

namespace load_bc3_srgba
{
	int test()
	{
		int Error(0);

		gli::texture2D TextureA(gli::load_dds("kueken7_bc3_srgba.dds"));
		gli::save_dds(TextureA, "kueken7_bc3_srgba_copy.dds");
		gli::texture2D TextureB(gli::load_dds("kueken7_bc3_srgba_copy.dds"));

		Error += TextureA == TextureB ? 0 : 1;

		return Error;
	}
}//namespace load_bc3_srgba

namespace load_etc2_rgba
{
	int test()
	{
		int Error(0);

		gli::texture2D TextureA(gli::load_dds("kueken7_etc2_rgb8.dds"));
		gli::save_dds(TextureA, "kueken7_etc2_rgb8_copy.dds");
		gli::texture2D TextureB(gli::load_dds("kueken7_etc2_rgb8_copy.dds"));

		Error += TextureA == TextureB ? 0 : 1;

		return Error;
	}
}//namespace load_etc2_rgba

namespace load_etc2_srgba
{
	int test()
	{
		int Error(0);

		gli::texture2D TextureA(gli::load_dds("kueken7_etc2_srgb8.dds"));
		gli::save_dds(TextureA, "kueken7_etc2_srgb8_copy.dds");
		gli::texture2D TextureB(gli::load_dds("kueken7_etc2_srgb8_copy.dds"));

		Error += TextureA == TextureB ? 0 : 1;

		return Error;
	}
}//namespace load_etc2_srgba

namespace load_r5g6b5_unorm
{
	int test()
	{
		int Error(0);

		gli::texture2D TextureA(gli::load_dds("kueken7_r5g6b5_unorm.dds"));
		gli::save_dds(TextureA, "kueken7_r5g6b5_unorm_copy.dds");
		gli::texture2D TextureB(gli::load_dds("kueken7_r5g6b5_unorm_copy.dds"));

		Error += TextureA == TextureB ? 0 : 1;

		return Error;
	}
}//namespace load_r5g6b5_unorm

namespace load_rg11b10
{
	int test()
	{
		int Error(0);

		gli::texture2D TextureA(gli::load_dds("kueken7_rg11b10_uf.dds"));
		gli::save_dds(TextureA, "kueken7_rg11b10_uf_copy.dds");
		gli::texture2D TextureB(gli::load_dds("kueken7_rg11b10_uf_copy.dds"));

		Error += TextureA == TextureB ? 0 : 1;

		return Error;
	}
}//namespace load_rg11b10

namespace load_rgb9e5
{
	int test()
	{
		int Error(0);

		gli::texture2D TextureA(gli::load_dds("kueken7_rgb9e5_uf.dds"));
		gli::save_dds(TextureA, "kueken7_rgb9e5_uf_copy.dds");
		gli::texture2D TextureB(gli::load_dds("kueken7_rgb9e5_uf_copy.dds"));

		Error += TextureA == TextureB ? 0 : 1;

		return Error;
	}
}//namespace load_rgb9e5

namespace load_srgb8_unorm
{
	int test()
	{
		int Error(0);

		gli::texture2D TextureA(gli::load_dds("kueken7_srgb8_unorm.dds"));
		gli::save_dds(TextureA, "kueken7_srgb8_unorm_copy.dds");
		gli::texture2D TextureB(gli::load_dds("kueken7_srgb8_unorm_copy.dds"));

		Error += TextureA == TextureB ? 0 : 1;

		return Error;
	}
}//namespace load_srgb8_unorm

namespace load_srgba8_unorm
{
	int test()
	{
		int Error(0);

		gli::texture2D TextureA(gli::load_dds("kueken7_srgba8_unorm.dds"));
		gli::save_dds(TextureA, "kueken7_srgba8_unorm_copy.dds");
		gli::texture2D TextureB(gli::load_dds("kueken7_srgba8_unorm_copy.dds"));

		Error += TextureA == TextureB ? 0 : 1;

		return Error;
	}
}//namespace load_srgba8_unorm

namespace load_rgba8_snorm
{
	int test()
	{
		int Error(0);

		gli::texture2D TextureA(gli::load_dds("kueken7_rgba8_snorm.dds"));
		gli::save_dds(TextureA, "kueken7_rgba8_snorm_copy.dds");
		gli::texture2D TextureB(gli::load_dds("kueken7_rgba8_snorm_copy.dds"));

		Error += TextureA == TextureB ? 0 : 1;

		return Error;
	}
}//namespace load_rgba8_snorm

namespace load_rgb10a2u
{
	int test()
	{
		int Error(0);

		gli::texture2D TextureA(gli::load_dds("kueken7_rgb10_a2_ui.dds"));
		gli::save_dds(TextureA, "kueken7_rgb10_a2_ui_copy.dds");
		gli::texture2D TextureB(gli::load_dds("kueken7_rgb10_a2_ui_copy.dds"));

		Error += TextureA == TextureB ? 0 : 1;

		return Error;
	}
}//namespace load_rgb10a2u

namespace load_rgb10a2_unorm
{
	int test()
	{
		int Error(0);

		gli::texture2D TextureA(gli::load_dds("kueken7_rgb10_a2_unorm.dds"));
		gli::save_dds(TextureA, "kueken7_rgb10_a2_unorm_copy.dds");
		gli::texture2D TextureB(gli::load_dds("kueken7_rgb10_a2_unorm_copy.dds"));

		Error += TextureA == TextureB ? 0 : 1;

		return Error;
	}
}//namespace load_rgb10a2_unorm

namespace load_astc8x8_rgb
{
	int test()
	{
		int Error(0);

		gli::texture2D TextureA(gli::load_dds("kueken7_astc8x8_rgb.dds"));
		gli::save_dds(TextureA, "kueken7_astc8x8_rgb_copy.dds");
		gli::texture2D TextureB(gli::load_dds("kueken7_astc8x8_rgb_copy.dds"));

		Error += TextureA == TextureB ? 0 : 1;

		return Error;
	}
}//namespace load_astc8x8_rgb

namespace load_pvrtc_2bpp
{
	int test()
	{
		int Error(0);

		gli::texture2D TextureA(gli::load_dds("kueken7_pvrtc_2bpp.dds"));
		gli::save_dds(TextureA, "kueken7_pvrtc_2bpp_copy.dds");
		gli::texture2D TextureB(gli::load_dds("kueken7_pvrtc_2bpp_copy.dds"));

		Error += TextureA == TextureB ? 0 : 1;

		return Error;
	}
}//namespace load_pvrtc_2bpp

namespace load_pvrtc_4bpp
{
	int test()
	{
		int Error(0);

		gli::texture2D TextureA(gli::load_dds("kueken7_pvrtc_4bpp.dds"));
		gli::save_dds(TextureA, "kueken7_pvrtc_4bpp_copy.dds");
		gli::texture2D TextureB(gli::load_dds("kueken7_pvrtc_4bpp_copy.dds"));

		Error += TextureA == TextureB ? 0 : 1;

		return Error;
	}
}//namespace load_pvrtc_4bpp

namespace load_pvrtc2_4bpp
{
	int test()
	{
		int Error(0);

		gli::texture2D TextureA(gli::load_dds("kueken7_pvrtc2_4bpp.dds"));
		gli::save_dds(TextureA, "kueken7_pvrtc2_4bpp_copy.dds");
		gli::texture2D TextureB(gli::load_dds("kueken7_pvrtc2_4bpp_copy.dds"));

		Error += TextureA == TextureB ? 0 : 1;

		return Error;
	}
}//namespace load_pvrtc2_4bpp

int main()
{
	int Error(0);

	Error += load_pvrtc_2bpp::test();
	Error += load_pvrtc_4bpp::test();
	//Error += load_astc8x8_rgb::test();
	//Error += load_pvrtc2_4bpp::test();
	//Error += load_rg11b10::test();
	//Error += load_rgb9e5::test();
	Error += load_ati1n::test();
	Error += load_ati2n::test();
	Error += load_bgr8::test();
	Error += load_dxt1::test();
	Error += load_dxt5::test();
	Error += load_bc4::test();
	Error += load_bc5::test();
	Error += load_bc7::test();
	Error += load_bgra8::test();
	Error += load_bgr8b::test();
	Error += load_bc1_rgb::test();
	Error += load_bc1_srgb::test();
	Error += load_bc3_rgba::test();
	Error += load_bc3_srgba::test();
	//Error += load_etc2_rgba::test();
	//Error += load_etc2_srgba::test();
	Error += load_r5g6b5_unorm::test();
	Error += load_srgb8_unorm::test();
	Error += load_srgba8_unorm::test();
	Error += load_rgba8_snorm::test();
	Error += load_rgb10a2u::test();
	Error += load_rgb10a2_unorm::test();

	return Error;
}
