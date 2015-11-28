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
/// @file gli/core/reduce.inl
/// @date 2015-11-27 / 2015-11-27
/// @author Christophe Riccio
///////////////////////////////////////////////////////////////////////////////////

namespace gli{
namespace detail
{
	template <typename vec_type>
	struct compute_reduce_1d
	{
		typedef typename reduce_func<vec_type>::type func_type;
		typedef texture1D::size_type size_type;
		typedef texture1D::texelcoord_type texelcoord_type;
		
		static vec_type call(texture1D const & A, texture1D const & B, func_type TexelFunc, func_type ReduceFunc)
		{
			GLI_ASSERT(all(equal(A.dimensions(), B.dimensions())));
			GLI_ASSERT(A.levels() == B.levels());
			GLI_ASSERT(A.size() == B.size());
		
			texelcoord_type TexelIndex(0);
			vec_type Result(TexelFunc(
				A.template load<vec_type>(TexelIndex, 0),
				B.template load<vec_type>(TexelIndex, 0)));
			
			for(size_type LevelIndex = 0, LevelCount = A.levels(); LevelIndex < LevelCount; ++LevelIndex)
			{
				texelcoord_type const TexelCount(A.dimensions(LevelIndex));
				for(TexelIndex.x = 0; TexelIndex.x < TexelCount.x; ++TexelIndex.x)
				{
					Result = ReduceFunc(Result, TexelFunc(
						A.template load<vec_type>(TexelIndex, LevelIndex),
						B.template load<vec_type>(TexelIndex, LevelIndex)));
				}
			}
			
			return Result;
		}
	};
	
	template <typename vec_type>
	struct compute_reduce_1d_array
	{
		typedef typename reduce_func<vec_type>::type func_type;
		typedef texture1DArray::size_type size_type;
		typedef texture1DArray::texelcoord_type texelcoord_type;
		
		static vec_type call(texture1DArray const & A, texture1DArray const & B, func_type TexelFunc, func_type ReduceFunc)
		{
			GLI_ASSERT(all(equal(A.dimensions(), B.dimensions())));
			GLI_ASSERT(A.levels() == B.levels());
			GLI_ASSERT(A.size() == B.size());
			
			texelcoord_type TexelIndex(0);
			vec_type Result(TexelFunc(
				A.template load<vec_type>(TexelIndex, 0),
				B.template load<vec_type>(TexelIndex, 0)));
			
			for(size_type LayerIndex = 0, LayerCount = A.layers(); LayerIndex < LayerCount; ++LayerIndex)
			for(size_type LevelIndex = 0, LevelCount = A.levels(); LevelIndex < LevelCount; ++LevelIndex)
			{
				texelcoord_type const TexelCount(A.dimensions(LevelIndex));
				for(TexelIndex.x = 0; TexelIndex.x < TexelCount.x; ++TexelIndex.x)
				{
					Result = ReduceFunc(Result, TexelFunc(
						A.template load<vec_type>(TexelIndex, LayerIndex, LevelIndex),
						B.template load<vec_type>(TexelIndex, LayerIndex, LevelIndex)));
				}
			}
			
			return Result;
		}
	};
	
	template <typename vec_type>
	struct compute_reduce_2d
	{
		typedef typename reduce_func<vec_type>::type func_type;
		typedef texture2D::size_type size_type;
		typedef texture2D::texelcoord_type texelcoord_type;
		
		static vec_type call(texture2D const & A, texture2D const & B, func_type TexelFunc, func_type ReduceFunc)
		{
			GLI_ASSERT(all(equal(A.dimensions(), B.dimensions())));
			GLI_ASSERT(A.levels() == B.levels());
			GLI_ASSERT(A.size() == B.size());
			
			texelcoord_type TexelIndex(0);
			vec_type Result(TexelFunc(
				A.template load<vec_type>(TexelIndex, 0),
				B.template load<vec_type>(TexelIndex, 0)));
			
			for(size_type LevelIndex = 0, LevelCount = A.levels(); LevelIndex < LevelCount; ++LevelIndex)
			{
				texelcoord_type const TexelCount(A.dimensions(LevelIndex));
				for(TexelIndex.y = 0; TexelIndex.y < TexelCount.y; ++TexelIndex.y)
				for(TexelIndex.x = 0; TexelIndex.x < TexelCount.x; ++TexelIndex.x)
				{
					Result = ReduceFunc(Result, TexelFunc(
						A.template load<vec_type>(TexelIndex, LevelIndex),
						B.template load<vec_type>(TexelIndex, LevelIndex)));
				}
			}
			
			return Result;
		}
	};
	
	template <typename vec_type>
	struct compute_reduce_2d_array
	{
		typedef typename reduce_func<vec_type>::type func_type;
		typedef texture2DArray::size_type size_type;
		typedef texture2DArray::texelcoord_type texelcoord_type;
		
		static vec_type call(texture2DArray const & A, texture2DArray const & B, func_type TexelFunc, func_type ReduceFunc)
		{
			GLI_ASSERT(all(equal(A.dimensions(), B.dimensions())));
			GLI_ASSERT(A.levels() == B.levels());
			GLI_ASSERT(A.size() == B.size());
			
			texelcoord_type TexelIndex(0);
			vec_type Result(TexelFunc(
				A.template load<vec_type>(TexelIndex, 0, 0),
				B.template load<vec_type>(TexelIndex, 0, 0)));
			
			for(size_type LayerIndex = 0, LayerCount = A.layers(); LayerIndex < LayerCount; ++LayerIndex)
			for(size_type LevelIndex = 0, LevelCount = A.levels(); LevelIndex < LevelCount; ++LevelIndex)
			{
				texelcoord_type const TexelCount(A.dimensions(LevelIndex));
				for(TexelIndex.y = 0; TexelIndex.y < TexelCount.y; ++TexelIndex.y)
				for(TexelIndex.x = 0; TexelIndex.x < TexelCount.x; ++TexelIndex.x)
				{
					Result = ReduceFunc(Result, TexelFunc(
						A.template load<vec_type>(TexelIndex, LayerIndex, LevelIndex),
						B.template load<vec_type>(TexelIndex, LayerIndex, LevelIndex)));
				}
			}
			
			return Result;
		}
	};
	
	template <typename vec_type>
	struct compute_reduce_3d
	{
		typedef typename reduce_func<vec_type>::type func_type;
		typedef texture3D::size_type size_type;
		typedef texture3D::texelcoord_type texelcoord_type;
		
		static vec_type call(texture3D const & A, texture3D const & B, func_type TexelFunc, func_type ReduceFunc)
		{
			GLI_ASSERT(all(equal(A.dimensions(), B.dimensions())));
			GLI_ASSERT(A.levels() == B.levels());
			GLI_ASSERT(A.size() == B.size());
			
			texelcoord_type TexelIndex(0);
			vec_type Result(TexelFunc(
				A.template load<vec_type>(TexelIndex, 0),
				B.template load<vec_type>(TexelIndex, 0)));
			
			for(size_type LevelIndex = 0, LevelCount = A.levels(); LevelIndex < LevelCount; ++LevelIndex)
			{
				texelcoord_type const TexelCount(A.dimensions(LevelIndex));
				for(TexelIndex.z = 0; TexelIndex.z < TexelCount.z; ++TexelIndex.z)
				for(TexelIndex.y = 0; TexelIndex.y < TexelCount.y; ++TexelIndex.y)
				for(TexelIndex.x = 0; TexelIndex.x < TexelCount.x; ++TexelIndex.x)
				{
					Result = ReduceFunc(Result, TexelFunc(
						A.template load<vec_type>(TexelIndex, LevelIndex),
						B.template load<vec_type>(TexelIndex, LevelIndex)));
				}
			}
			
			return Result;
		}
	};
	
	template <typename vec_type>
	struct compute_reduce_cube
	{
		typedef typename reduce_func<vec_type>::type func_type;
		typedef textureCube::size_type size_type;
		typedef textureCube::texelcoord_type texelcoord_type;
		
		static vec_type call(textureCube const & A, textureCube const & B, func_type TexelFunc, func_type ReduceFunc)
		{
			GLI_ASSERT(all(equal(A.dimensions(), B.dimensions())));
			GLI_ASSERT(A.levels() == B.levels());
			GLI_ASSERT(A.size() == B.size());
			
			texelcoord_type TexelIndex(0);
			vec_type Result(TexelFunc(
				A.load<vec_type>(TexelIndex, 0, 0),
				B.load<vec_type>(TexelIndex, 0, 0)));
			
			for(size_type FaceIndex = 0, FaceCount = A.faces(); FaceIndex < FaceCount; ++FaceIndex)
			for(size_type LevelIndex = 0, LevelCount = A.levels(); LevelIndex < LevelCount; ++LevelIndex)
			{
				texelcoord_type const TexelCount(A.dimensions(LevelIndex));
				for(TexelIndex.y = 0; TexelIndex.y < TexelCount.y; ++TexelIndex.y)
				for(TexelIndex.x = 0; TexelIndex.x < TexelCount.x; ++TexelIndex.x)
				{
					Result = ReduceFunc(Result, TexelFunc(
						A.template load<vec_type>(TexelIndex, FaceIndex, LevelIndex),
						B.template load<vec_type>(TexelIndex, FaceIndex, LevelIndex)));
				}
			}
			
			return Result;
		}
	};
	
	template <typename vec_type>
	struct compute_reduce_cube_array
	{
		typedef typename reduce_func<vec_type>::type func_type;
		typedef textureCubeArray::size_type size_type;
		typedef textureCubeArray::texelcoord_type texelcoord_type;
		
		static vec_type call(textureCubeArray const & A, textureCubeArray const & B, func_type TexelFunc, func_type ReduceFunc)
		{
			GLI_ASSERT(all(equal(A.dimensions(), B.dimensions())));
			GLI_ASSERT(A.levels() == B.levels());
			GLI_ASSERT(A.size() == B.size());
			
			texelcoord_type TexelIndex(0);
			vec_type Result(TexelFunc(
				A.load<vec_type>(TexelIndex, 0, 0, 0),
				B.load<vec_type>(TexelIndex, 0, 0 ,0)));
			
			for(size_type LayerIndex = 0, LayerCount = A.layers(); LayerIndex < LayerCount; ++LayerIndex)
			for(size_type FaceIndex = 0, FaceCount = A.faces(); FaceIndex < FaceCount; ++FaceIndex)
			for(size_type LevelIndex = 0, LevelCount = A.levels(); LevelIndex < LevelCount; ++LevelIndex)
			{
				texelcoord_type const TexelCount(A.dimensions(LevelIndex));
				for(TexelIndex.y = 0; TexelIndex.y < TexelCount.y; ++TexelIndex.y)
				for(TexelIndex.x = 0; TexelIndex.x < TexelCount.x; ++TexelIndex.x)
				{
					Result = ReduceFunc(Result, TexelFunc(
						A.template load<vec_type>(TexelIndex, LayerIndex, FaceIndex, LevelIndex),
						B.template load<vec_type>(TexelIndex, LayerIndex, FaceIndex, LevelIndex)));
				}
			}
			
			return Result;
		}
	};
	
}//namepsace detail

template <typename vec_type>
inline vec_type reduce(texture1D const & In0, texture1D const & In1, typename reduce_func<vec_type>::type TexelFunc, typename reduce_func<vec_type>::type ReduceFunc)
{
	return detail::compute_reduce_1d<vec_type>::call(In0, In1, TexelFunc, ReduceFunc);
}

template <typename vec_type>
inline vec_type reduce(texture1DArray const & In0, texture1DArray const & In1, typename reduce_func<vec_type>::type TexelFunc, typename reduce_func<vec_type>::type ReduceFunc)
{
	return detail::compute_reduce_1d_array<vec_type>::call(In0, In1, TexelFunc, ReduceFunc);
}

template <typename vec_type>
inline vec_type reduce(texture2D const & In0, texture2D const & In1, typename reduce_func<vec_type>::type TexelFunc, typename reduce_func<vec_type>::type ReduceFunc)
{
	return detail::compute_reduce_2d<vec_type>::call(In0, In1, TexelFunc, ReduceFunc);
}

template <typename vec_type>
inline vec_type reduce(texture2DArray const & In0, texture2DArray const & In1, typename reduce_func<vec_type>::type TexelFunc, typename reduce_func<vec_type>::type ReduceFunc)
{
	return detail::compute_reduce_2d_array<vec_type>::call(In0, In1, TexelFunc, ReduceFunc);
}

template <typename vec_type>
inline vec_type reduce(texture3D const & In0, texture3D const & In1, typename reduce_func<vec_type>::type TexelFunc, typename reduce_func<vec_type>::type ReduceFunc)
{
	return detail::compute_reduce_3d<vec_type>::call(In0, In1, TexelFunc, ReduceFunc);
}

template <typename vec_type>
inline vec_type reduce(textureCube const & In0, textureCube const & In1, typename reduce_func<vec_type>::type TexelFunc, typename reduce_func<vec_type>::type ReduceFunc)
{
	return detail::compute_reduce_cube<vec_type>::call(In0, In1, TexelFunc, ReduceFunc);
}

template <typename vec_type>
inline vec_type reduce(textureCubeArray const & In0, textureCubeArray const & In1, typename reduce_func<vec_type>::type TexelFunc, typename reduce_func<vec_type>::type ReduceFunc)
{
	return detail::compute_reduce_cube_array<vec_type>::call(In0, In1, TexelFunc, ReduceFunc);
}
}//namespace gli

