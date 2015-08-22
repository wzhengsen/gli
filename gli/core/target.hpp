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
/// @file gli/core/target.hpp
/// @date 2015-08-21 / 2015-08-21
/// @author Christophe Riccio
///////////////////////////////////////////////////////////////////////////////////

namespace gli
{
	enum target
	{
		TARGET_1D,
		TARGET_1D_ARRAY,
		TARGET_2D,
		TARGET_2D_ARRAY,
		TARGET_3D,
		TARGET_CUBE,
		TARGET_CUBE_ARRAY, TARGET_MAX = TARGET_CUBE_ARRAY
	};

	enum
	{
		TARGET_COUNT = TARGET_MAX + 1,
		TARGET_INVALID = -1
	};

	inline bool isTarget1D(target Target)
	{
		return Target == TARGET_1D || Target == TARGET_1D_ARRAY;
	}

	inline bool isTargetArray(target Target)
	{
		return Target == TARGET_1D_ARRAY || Target == TARGET_2D_ARRAY || Target == TARGET_CUBE_ARRAY;
	}

	inline bool isTargetCube(target Target)
	{
		return Target == TARGET_CUBE || Target == TARGET_CUBE_ARRAY;
	}
}//namespace gli
