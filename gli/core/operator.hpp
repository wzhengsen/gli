#pragma once

#include "texture2d.hpp"

namespace gli{
namespace detail
{

}//namespace detail

	/// Add two textures, each texel
	texture2d operator+(texture2d const & TextureA, texture2d const & TextureB);

	/// Substract two textures, each texel
	texture2d operator-(texture2d const & TextureA, texture2d const & TextureB);

}//namespace gli

#include "operator.inl"
