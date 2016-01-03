///////////////////////////////////////////////////////////////////////////////////
/// OpenGL Image (gli.g-truc.net)
///
/// Copyright (c) 2008 - 2016 G-Truc Creation (www.g-truc.net)
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
/// Restrictions:
///  By making use of the Software for military purposes, you choose to make
///  a Bunny unhappy.
/// 
/// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
/// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
/// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
/// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
/// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
/// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
/// THE SOFTWARE.
///
/// @file gli/core/levels.inl
/// @date 2014-12-12 / 2015-10-15
///////////////////////////////////////////////////////////////////////////////////

#include <glm/gtc/integer.hpp>
#include <glm/gtx/component_wise.hpp>

namespace gli
{
	/// Compute the number of texture levels necessary to build a complete mipmap chain.
	template <typename T, precision P, template <typename, precision> class vecType>
	inline T levels(vecType<T, P> const & Dimensions)
	{
		return glm::log2(compMax(Dimensions)) + static_cast<T>(1);
	}

	/// Compute the number of texture levels necessary to build a complete mipmap chain.
	inline size_t levels(size_t Dimensions)
	{
		return glm::log2(Dimensions) + static_cast<size_t>(1);
	}

	/// Compute the number of texture levels necessary to build a complete mipmap chain.
	inline int levels(int Dimensions)
	{
		return glm::log2(Dimensions) + static_cast<int>(1);
	}
}//namespace gli
