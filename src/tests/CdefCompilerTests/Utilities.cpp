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

#include "stdafx.h"
#include "../../libCdefCompiler/Utilities.hpp"

// Validate that the AST is working as expected
BOOST_AUTO_TEST_SUITE(Utilities);

using namespace lib_perf_counter;

BOOST_AUTO_TEST_CASE( FindFileInPath01 )
{
#if defined(_WIN32)
	std::string path = findInEnvironmentPath("CMD.EXE");
	BOOST_CHECK(path.find("CMD.EXE") != std::string::npos);
#endif
}

BOOST_AUTO_TEST_CASE(FindFileInPath02)
{
#if defined(_WIN32)
	std::string path = findInEnvironmentPath("edsf44g446");
	BOOST_CHECK(path.find("edsf44g446") == std::string::npos);
#endif
}

BOOST_AUTO_TEST_CASE(shellExecute01)
{
#if defined(_WIN32)
	uint32_t result = shellExecute("dir", "/b", "NUL:", "NUL:");

	BOOST_CHECK(result == 0);
#endif
}

BOOST_AUTO_TEST_SUITE_END()
