#include <gli/gl.hpp>

namespace gl_core
{
	int run()
	{
		int Error = 0;

		gli::gl GL(gli::gl::PROFILE_CORE);

		{
			gli::gl::format Format = GL.translate(gli::FORMAT_RGB8_UNORM_PACK8);

			Error += Format.Internal == gli::gl::INTERNAL_RGB8_UNORM ? 0 : 1;
			Error += Format.External == gli::gl::EXTERNAL_RGB ? 0 : 1;
			Error += Format.Type == gli::gl::TYPE_U8 ? 0 : 1;
		}

		{
			gli::gl::format Format = GL.translate(gli::FORMAT_BGR8_UNORM_PACK8);

			Error += Format.Internal == gli::gl::INTERNAL_RGB8_UNORM ? 0 : 1;
			Error += Format.External == gli::gl::EXTERNAL_BGR ? 0 : 1;
			Error += Format.Type == gli::gl::TYPE_U8 ? 0 : 1;
		}

		return Error;
	}
}//namespace gl_core

int main()
{
	int Error(0);

	Error += gl_core::run();

	return Error;
}
