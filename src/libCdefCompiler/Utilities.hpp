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

#ifndef __CDEF_COMPILER_UTILITIES_HPP_
#define __CDEF_COMPILER_UTILITIES_HPP_

#include <string>

namespace lib_perf_counter {

// These function will only work and can only be compiled under windows
// the following code is *very* windows specific
#ifdef _WIN32

// name of Microsoft provided pre-processor tool
#define CTRPP_EXE	"ctrpp.exe"

/**
 * Searches for the first CTRPP executable.
 *
 * \return	The path to the CTRPP executable if it could be found, the empty string otherwise-
 */
extern std::string findCtrppExe();

/**
 * Query if 'fileName' is in path.
 *
 * \param	fileName	Filename of the file.
 * \return	true if in path, false if not.
 */
extern std::string findInEnvironmentPath(const std::string& fileName);

/**
 * Execute a program in a new shell, similar to fork.
 *
 * \param	program   	The name and path to the program to execute
 * \param	parameters	Options for controlling the operation.
 * \param	consoleOut	Redirect the console output to this file
 * \param	errorOut  	Redirect the console error output to this file.
 *
 * \return	The error code resulting from running the program.
 */

extern uint32_t shellExecute(const std::string& program, const std::string& parameters, const std::string& consoleOut, const std::string& errorOut);

#endif
}
#endif // !__CDEF_COMPILER_UTILITIES_HPP_

