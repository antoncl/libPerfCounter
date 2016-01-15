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

#ifndef __CDEF_COMPILER_CONFIGURATION_HPP_
#define __CDEF_COMPILER_CONFIGURATION_HPP_

#include <iostream>
#include <memory>

#include <boost/program_options.hpp>
#include <boost/filesystem.hpp>

namespace lib_perf_counter {
namespace po = boost::program_options;
namespace fs = boost::filesystem;

class Configuration
{
public:
	Configuration(int argc, char* argv[], std::ostream& out = std::cout, std::ostream& err = std::cerr);

	bool getShouldExit() const { return _shouldExit; } 
	int  getExitCode() const { return _exitCode; } 

	const fs::path getInputFile() const { return _map["input-file"].as<std::vector<fs::path>>()[0];	}
	const fs::path getCtrppLocation() const { return _ctrppLocation; }
	const fs::path getRcLocation() const { return _rcLocation; }
	const fs::path getHppLocation() const { return _hppLocation; }
	const fs::path getManLocation() const { return _manLocation; }
	const fs::path getImplLocation() const { return _implLocation; }

	static Configuration& get() { return *_singleton; }

private:
	po::options_description getGenericOptions();
	po::options_description getHiddenOptions();
	po::options_description getConfigFileOptions();
	bool isSane(std::ostream& out, std::ostream& err);

	po::variables_map _map;
	fs::path _ctrppLocation;
	fs::path _rcLocation;
	fs::path _hppLocation;
	fs::path _manLocation;
	fs::path _implLocation;
	bool _shouldExit;
	int  _exitCode;

	static Configuration* _singleton;
};

}
#endif // !__CDEF_COMPILER_CONFIGURATION_HPP_
