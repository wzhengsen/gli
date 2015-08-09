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
/// @file gli/core/load.inl
/// @date 2010-09-26 / 2015-06-16
/// @author Christophe Riccio
///////////////////////////////////////////////////////////////////////////////////

#include "load_dds.hpp"
#include "load_ktx.hpp"

namespace gli{

inline storage load(char const * Data, std::size_t Size)
{
	{
		storage Storage = load_dds(Data, Size);
		if(!Storage.empty())
			return Storage;
	}
	{
		return load_ktx(Data, Size);
	}
}

inline storage load(char const * Filename)
{
	FILE* File = std::fopen(Filename, "rb");
	assert(File);

	long Beg = std::ftell(File);
	std::fseek(File, 0, SEEK_END);
	long End = std::ftell(File);
	std::fseek(File, 0, SEEK_SET);

	std::vector<char> Data(static_cast<std::size_t>(End - Beg));

	std::fread(&Data[0], 1, Data.size(), File);
	std::fclose(File);

	return load(&Data[0], Data.size());
}

inline storage load(std::string const & Filename)
{
	return load(Filename.c_str());
}

}//namespace gli
