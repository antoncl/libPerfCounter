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

// CounterGen.cpp : Defines the entry point for the console application.
//

#include <exception>
#include <iostream>
#include <boost/filesystem.hpp>

#include "../libCdefCompiler/Compiler.hpp"
#include "../libCdefCompiler/Configuration.hpp"


int main(int argc, char* argv[])
{
	try
	{
		lib_perf_counter::Configuration configuration(argc, argv);

		if (configuration.getShouldExit())
			return configuration.getExitCode();

		lib_perf_counter::Compiler compiler(configuration.getInputFile());
		if(!compiler.Run())
			return 2;
	}
	catch (std::exception& e) {
		std::cerr << e.what() << std::endl;
		return 2;
	}
	return 0;
}
