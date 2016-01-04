/// @brief Include to load KMG textures from files or memory.
/// @file gli/load_kmg.hpp

#pragma once

#include "texture.hpp"

namespace gli
{
	/// Loads a texture storage from KMG (Khronos Image) file. Returns an empty storage in case of failure.
	///
	/// @param Path Path of the file to open including filaname and filename extension
	texture load_kmg(char const * Path);

	/// Loads a texture storage from KMG (Khronos Image) file. Returns an empty storage in case of failure.
	///
	/// @param Path Path of the file to open including filaname and filename extension
	texture load_kmg(std::string const & Filename);

	/// Loads a texture storage from KMG (Khronos Image) memory. Returns an empty storage in case of failure.
	///
	/// @param Path Path of the file to open including filaname and filename extension
	texture load_kmg(char const * Data, std::size_t Size);
}//namespace gli

#include "./core/load_kmg.inl"
