#pragma once

#include "../texture2d.hpp"

namespace gli
{
/*
	texture2d duplicate(texture2d const & Texture);
	texture2d flip(texture2d const & Texture);
	texture2d mirror(texture2d const & Texture);
	texture2d swizzle(
		texture2d const & Texture, 
		glm::uvec4 const & Channel);
	texture2d crop(
		texture2d const & Texture, 
		texture2d::dimensions_type const & Position,
		texture2d::dimensions_type const & Size);

	image2D crop(
		image2D const & Image, 
		image2D::dimensions_type const & Position,
		image2D::dimensions_type const & Size);

	image2D copy(
		image2D const & SrcImage, 
		image2D::dimensions_type const & SrcPosition,
		image2D::dimensions_type const & SrcSize,
		image2D & DstImage, 
		image2D::dimensions_type const & DstPosition);
*/
	//image operator+(image const & MipmapA, image const & MipmapB);
	//image operator-(image const & MipmapA, image const & MipmapB);
	//image operator*(image const & MipmapA, image const & MipmapB);
	//image operator/(image const & MipmapA, image const & MipmapB);

	//namespace wip
	//{
	//	template <typename GENTYPE, template <typename> class SURFACE>
	//	GENTYPE fetch(SURFACE<GENTYPE> const & Image)
	//	{
	//		return GENTYPE();
	//	}

	//	template
	//	<
	//		typename GENTYPE, 
	//		template 
	//		<
	//			typename
	//		>
	//		class SURFACE,
	//		template 
	//		<
	//			typename, 
	//			template 
	//			<
	//				typename
	//			>
	//			class
	//		> 
	//		class IMAGE
	//	>
	//	GENTYPE fetch(IMAGE<GENTYPE, SURFACE> const & Image)
	//	{
	//		return GENTYPE();
	//	}
	//}//namespace wip

}//namespace gli

#include "operation.inl"
