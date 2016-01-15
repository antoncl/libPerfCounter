// Copyright (C) 2015,2016 Anton Lauridsen
//
// This file is part of libPerfCounter.
//
// libPerfCounter is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as
// published by the Free Software Foundation, either version 3 of the
// License, or (at your option) any later version.
//
// libPerfCounter is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public
// License along with libPerfCounter. If not, see <http://www.gnu.org/licenses/>.

#ifndef _COMPILER_HPP
#define _COMPILER_HPP

/* MSVC  linker settings. */
#if defined(_MSC_VER)
#    if defined(_M_X64)
#        ifdef _DEBUG
#            pragma comment(lib, "libCdefCompilerd.lib")
#        else
#            pragma comment(lib, "libCdefCompiler.lib")
#        endif
#    endif
#    if defined(_M_IX86)
#        ifdef _DEBUG
#            pragma comment(lib, "libCdefCompilerd.lib")
#        else
#            pragma comment(lib, "libCdefCompiler.lib")
#        endif
#    endif
#    pragma comment(lib, "Kernel32.lib")
#endif

#include <boost/filesystem.hpp>
#include <boost/property_tree/ptree.hpp>
#include <string>

#include "AstProvider.hpp"

namespace lib_perf_counter {

namespace pt = ::boost::property_tree;
namespace fs = ::boost::filesystem;

/**
 * Parse and generate counter definitions
 */
class Compiler
{
public:
	/**
	 * Parse constructor
	 * Load and parse the counter definitions file.
	 * RAII based constructor.
	 *
	 * @param sourceFileName	Filename and possibly path of the file to parse
	 * @throw exception			Thrown on missing file etc.
	 */
	Compiler(const fs::path& sourceFileName);

	/**
	 * Perform the actual code generation
	 * @return true if the generation succeeded, false otherwise
	 */
	bool Run();

private:
	fs::path _sourceFileName;

	void errorHandler(const CompileMessage& message);
};

}
#endif//_COMPILER_HPP
