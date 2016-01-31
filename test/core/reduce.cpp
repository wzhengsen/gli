#include <gli/reduce.hpp>
#include <glm/vector_relational.hpp>

namespace
{
	inline gli::u8vec4 abs_diff(gli::u8vec4 const & A, gli::u8vec4 const & B)
	{
		return gli::abs(A - B);
	}
}

namespace reduce1d
{
	int test()
	{
		int Error = 0;
		
		gli::texture1D TextureA(gli::FORMAT_RGBA8_UNORM_PACK8, gli::texture1D::texelcoord_type(32));
		TextureA.clear(gli::u8vec4(255, 127, 0, 255));
		gli::texture1D TextureB(gli::FORMAT_RGBA8_UNORM_PACK8, gli::texture1D::texelcoord_type(32));
		TextureB.clear(gli::u8vec4(255, 127, 0, 255));
		
		gli::u8vec4 const MaxAbsDiff = gli::reduce<gli::u8vec4>(TextureA, TextureB, abs_diff, gli::max);
		Error += gli::all(glm::equal(MaxAbsDiff, gli::u8vec4(0))) ? 0 : 1;
		
		return Error;
	}
}//namespace reduce1d

namespace reduce1d_array
{
	int test()
	{
		int Error = 0;
		
		gli::texture1D TextureA(gli::FORMAT_RGBA8_UNORM_PACK8, gli::texture1D::texelcoord_type(32));
		TextureA.clear(gli::u8vec4(255, 127, 0, 255));
		gli::texture1D TextureB(gli::FORMAT_RGBA8_UNORM_PACK8, gli::texture1D::texelcoord_type(32));
		TextureB.clear(gli::u8vec4(255, 127, 0, 255));
		
		gli::u8vec4 const MaxAbsDiff = gli::reduce<gli::u8vec4>(TextureA, TextureB, abs_diff, gli::max);
		Error += gli::all(glm::equal(MaxAbsDiff, gli::u8vec4(0))) ? 0 : 1;
		
		return Error;
	}
}//namespace reduce1d_array

namespace reduce2d
{
	int test()
	{
		int Error = 0;
		
		gli::texture2D TextureA(gli::FORMAT_RGBA8_UNORM_PACK8, gli::texture2D::texelcoord_type(32));
		TextureA.clear(gli::u8vec4(255, 127, 0, 255));
		gli::texture2D TextureB(gli::FORMAT_RGBA8_UNORM_PACK8, gli::texture2D::texelcoord_type(32));
		TextureB.clear(gli::u8vec4(255, 127, 0, 255));
		
		gli::u8vec4 const MaxAbsDiff = gli::reduce<gli::u8vec4>(TextureA, TextureB, abs_diff, gli::max);
		Error += gli::all(glm::equal(MaxAbsDiff, gli::u8vec4(0))) ? 0 : 1;
		
		return Error;
	}
}//namespace reduce2d

namespace reduce2d_array
{
	int test()
	{
		int Error = 0;
		
		gli::texture2D TextureA(gli::FORMAT_RGBA8_UNORM_PACK8, gli::texture2D::texelcoord_type(32));
		TextureA.clear(gli::u8vec4(255, 127, 0, 255));
		gli::texture2D TextureB(gli::FORMAT_RGBA8_UNORM_PACK8, gli::texture2D::texelcoord_type(32));
		TextureB.clear(gli::u8vec4(255, 127, 0, 255));
		
		gli::u8vec4 const MaxAbsDiff = gli::reduce<gli::u8vec4>(TextureA, TextureB, abs_diff, gli::max);
		Error += gli::all(glm::equal(MaxAbsDiff, gli::u8vec4(0))) ? 0 : 1;
		
		return Error;
	}
}//namespace reduce2d_array

namespace reduce3d
{
	int test()
	{
		int Error = 0;
		
		gli::texture3D TextureA(gli::FORMAT_RGBA8_UNORM_PACK8, gli::texture3D::texelcoord_type(32));
		TextureA.clear(gli::u8vec4(255, 127, 0, 255));
		gli::texture3D TextureB(gli::FORMAT_RGBA8_UNORM_PACK8, gli::texture3D::texelcoord_type(32));
		TextureB.clear(gli::u8vec4(255, 127, 0, 255));
		
		gli::u8vec4 const MaxAbsDiff = gli::reduce<gli::u8vec4>(TextureA, TextureB, abs_diff, gli::max);
		Error += gli::all(glm::equal(MaxAbsDiff, gli::u8vec4(0))) ? 0 : 1;
		
		return Error;
	}
}//namespace reduce3d

namespace reduce_cube
{
	int test()
	{
		int Error = 0;
		
		gli::textureCube TextureA(gli::FORMAT_RGBA8_UNORM_PACK8, gli::textureCube::texelcoord_type(32));
		TextureA.clear(gli::u8vec4(255, 127, 0, 255));
		gli::textureCube TextureB(gli::FORMAT_RGBA8_UNORM_PACK8, gli::textureCube::texelcoord_type(32));
		TextureB.clear(gli::u8vec4(255, 127, 0, 255));
		
		gli::u8vec4 const MaxAbsDiff = gli::reduce<gli::u8vec4>(TextureA, TextureB, abs_diff, gli::max);
		Error += gli::all(glm::equal(MaxAbsDiff, gli::u8vec4(0))) ? 0 : 1;
		
		return Error;
	}
}//namespace reduce_cube

namespace reduce_cube_array
{
	int test()
	{
		int Error = 0;
		
		gli::textureCubeArray TextureA(gli::FORMAT_RGBA8_UNORM_PACK8, gli::textureCubeArray::texelcoord_type(32), 2);
		TextureA.clear(gli::u8vec4(255, 127, 0, 255));
		gli::textureCubeArray TextureB(gli::FORMAT_RGBA8_UNORM_PACK8, gli::textureCubeArray::texelcoord_type(32), 2);
		TextureB.clear(gli::u8vec4(255, 127, 0, 255));
		
		gli::u8vec4 const MaxAbsDiff = gli::reduce<gli::u8vec4>(TextureA, TextureB, abs_diff, gli::max);
		Error += gli::all(glm::equal(MaxAbsDiff, gli::u8vec4(0))) ? 0 : 1;
		
		return Error;
	}
}//namespace reduce_cube_array

int main()
{
	int Error = 0;

	Error += reduce1d::test();
	Error += reduce1d_array::test();
	Error += reduce2d::test();
	Error += reduce2d_array::test();
	Error += reduce3d::test();
	Error += reduce_cube::test();
	Error += reduce_cube_array::test();

	return Error;
}
