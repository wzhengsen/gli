#include <gli/copy.hpp>
#include <gli/texture2d.hpp>

int main()
{
	int Error = 0;

	gli::texture2d Source(gli::FORMAT_R8_UNORM_PACK8, gli::extent2d(4, 4), 1);
	Source.clear(gli::u8(0));
	Source.store(gli::texture2d::extent_type(1, 1), 0, gli::u8(1));
	Source.store(gli::texture2d::extent_type(2, 1), 0, gli::u8(2));
	Source.store(gli::texture2d::extent_type(2, 2), 0, gli::u8(3));
	Source.store(gli::texture2d::extent_type(1, 2), 0, gli::u8(4));

	gli::texture2d Destination(Source.format(), Source.extent(), Source.levels());
	Destination.clear(gli::u8(255));

	Destination.copy(Source, 0, 0, 0, gli::texture::extent_type(1, 1, 0), 0, 0, 0, gli::texture::extent_type(1, 1, 0), gli::texture::extent_type(2, 2, 1));
	for(gli::size_t IndexY = 1; IndexY < 3; ++IndexY)
	for(gli::size_t IndexX = 1; IndexX < 3; ++IndexX)
		Error += Source.load<gli::u8>(gli::texture2d::extent_type(IndexX, IndexY), 0) == Destination.load<gli::u8>(gli::texture2d::extent_type(IndexX, IndexY), 0) ? 0 : 1;

	return Error;
}


