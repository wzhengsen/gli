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
/// @file gli/core/standard_deviation.inl
/// @date 2015-11-26 / 2015-11-26
/// @author Christophe Riccio
///////////////////////////////////////////////////////////////////////////////////

namespace gli{
namespace detail
{
	template <typename texture_type, typename vec_type>
	struct compute_max_diff
	{};
	
	template <typename vec_type>
	struct compute_max_diff<texture2D, vec_type>
	{
		static vec_type call(texture2D const & A, texture2D const & B)
		{
			typedef typename texture2D::texelcoord_type texelcoord_type;
			
			texelcoord_type const TexelIndex(0), TexelCount(A.template texture::dimensions(0));
			
			GLI_ASSERT(all(equal(A.dimensions(), B.dimensions())));
			
			for(typename ; TexelIndex.z < TexelCount.z; ++TexelIndex.z)
				
			for(; TexelIndex.z < TexelCount.z; ++TexelIndex.z)
			for(; TexelIndex.y < TexelCount.y; ++TexelIndex.y)
			for(; TexelIndex.x < TexelCount.x; ++TexelIndex.x)
			{
				A.load<vec_type>(TexelIndex, );
			}
		}
	};
}//namespace detail
	
	template <typename texture_type, typename vec_type>
	inline vecType max_diff(texture_type const & A, texture_type const & B)
	{
		return detail::compute_max_diff<texture_type, vec_type>::call(A, B);
	}

}//namespace gli

