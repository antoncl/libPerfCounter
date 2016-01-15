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

#include "Version.hpp"
#include "Utilities.hpp"
#include "Configuration.hpp"

namespace lib_perf_counter {

Configuration* Configuration::_singleton = nullptr;

po::options_description Configuration::getGenericOptions()
{
	po::options_description options("Generic options");
	options.add_options()
		("version,v", "print version string")
		("help,h", "produce help message")
#if defined(_WIN32)
		("ctrpp-location,c",	 po::value<fs::path>(&_ctrppLocation),	"location of " CTRPP_EXE )
		("rc-out,r",			 po::value<fs::path>(&_rcLocation),		"filename of the generated resource file")
		("header-out,o",		 po::value<fs::path>(&_hppLocation),	"filename of the generated header file")
		("manifest-out,m",		 po::value<fs::path>(&_hppLocation),	"filename of the generated manifest file")
		("implementation-out,i", po::value<fs::path>(&_hppLocation),	"filename of the generated implementation file")
#endif
		;
	return options;
}

po::options_description Configuration::getHiddenOptions()
{
	po::options_description options("Hidden options");
	options.add_options()
		("input-file", po::value< std::vector<fs::path> >(), "input file")
		;
	return options;
}

po::options_description Configuration::getConfigFileOptions()
{
	po::options_description options("Configuration");
	options.add_options()
#if defined(_WIN32)
		("ctrpp-location,c", po::value<fs::path>(&_ctrppLocation), "location of " CTRPP_EXE)
#endif
		;
	return options;
}

Configuration::Configuration(int argc, char* argv[], std::ostream& out, std::ostream& err) :
	_map(),
	_ctrppLocation(),
	_rcLocation(),
	_hppLocation(),
	_shouldExit(false),
	_exitCode(0)
{
	std::string config_file;

	// Declare a group of options that will be 
	// allowed only on command line
	po::options_description generic = getGenericOptions();


	// Hidden options, will be allowed both on command line and
	// in config file, but will not be shown to the user.
	po::options_description hidden = getHiddenOptions();

	po::options_description config = getConfigFileOptions();

	po::options_description cmdLineOptions;
	cmdLineOptions.add(generic).add(hidden);

	po::options_description configFileOptions;
	configFileOptions.add(config).add(hidden);

	po::options_description visible; // ("Allows options");
	visible.add(generic).add(config);

	po::positional_options_description p;
	p.add("input-file", -1);

	po::store(po::command_line_parser(argc, argv).options(cmdLineOptions).positional(p).run(), _map);

	po::notify(_map);

	if (_map.count("help")) {
		out << "Usage: cdefcompiler [options] input-file\n";
		out << visible << std::endl;
		_shouldExit = true;
	}
	else if (_map.count("version")) {
		out << AppVersion.str() << std::endl;
		_shouldExit = true;
	}

	if (!_shouldExit)
		_shouldExit = !isSane(out, err);

	_singleton = this;
}

bool Configuration::isSane(std::ostream& /*out*/, std::ostream& err)
{
	// if we didn't get a file to parse, then simply quit
	if (!_map.count("input-file")) {
		err << "No file to parse was specified" << std::endl;
		_exitCode = 2;
		return false;
	}

	std::vector<fs::path> inputFiles = _map["input-file"].as<std::vector<fs::path>>();
	if (inputFiles.size() != 1) {
		err << "Warning! definitions will only be generated for the first file specified" << std::endl;
	}

	return true;
}

}