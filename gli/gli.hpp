/// @brief Include to include everything in GLI which is not recommendated due to compilation time cost.
/// @file gli/gli.hpp

#pragma once

#define GLI_VERSION					80
#define GLI_VERSION_MAJOR			0
#define GLI_VERSION_MINOR			8
#define GLI_VERSION_PATCH			0
#define GLI_VERSION_REVISION		0

/// Namespace where all the classes and functions provided by GLI are exposed
namespace gli
{

}//namespace gli

#include "format.hpp"
#include "target.hpp"
#include "levels.hpp"

#include "image.hpp"
#include "texture.hpp"
#include "texture1d.hpp"
#include "texture1d_array.hpp"
#include "texture2d.hpp"
#include "texture2d_array.hpp"
#include "texture3d.hpp"
#include "texture_cube.hpp"
#include "texture_cube_array.hpp"

#include "sampler1d.hpp"
#include "sampler1d_array.hpp"
#include "sampler2d.hpp"
#include "sampler2d_array.hpp"
#include "sampler3d.hpp"
#include "sampler_cube.hpp"
#include "sampler_cube_array.hpp"

#include "copy.hpp"
#include "view.hpp"
#include "comparison.hpp"

#include "reduce.hpp"
#include "transform.hpp"

#include "load.hpp"
#include "save.hpp"

#include "gl.hpp"
#include "dx.hpp"

#include "./core/flip.hpp"
