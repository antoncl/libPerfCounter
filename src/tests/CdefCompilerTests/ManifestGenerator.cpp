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

#include <boost/property_tree/info_parser.hpp>
namespace utf = boost::unit_test;

#include "../../libCdefCompiler/AstProvider.hpp"
#include "../../libCdefCompiler/DictionaryBuilder.hpp"
#include "../../libCdefCompiler/ManifestGenerator.hpp"
#include "../../libCdefCompiler/Configuration.hpp"

// Validate that the AST is working as expected
BOOST_AUTO_TEST_SUITE(ManifestGenerator);

using namespace lib_perf_counter;

BOOST_AUTO_TEST_CASE(ManifestGenerator01)
{
	istringstream source("provider test {\n"
		"application test.exe\n"
		"uuid 4417944E-470D-488D-9265-122B84BFE192\n"
		"counterset set1\n{"
		"uuid CADFF40D-91D8-41B3-8A78-9FA587DBD137\n"
		"description \"some description\"\n"
		"counter c1 {\n"
		"id 1\n"
		"name \"name1\"\n"
		"description \"some other description\"\n"
		"type Gauge32\n"
		"}\n"
		"}\n"
		"}\n");

	pt::ptree tree;
	pt::read_info(source, tree);

	AstProviderPtr ast = AstProvider::create();

	BOOST_CHECK(ast->parse(tree, [](const CompileMessage& message) {
		BOOST_ERROR("Unexpected compile error:" + message.getMessage());
	}));

	DictionaryBuilder dictBuilder;
	ast->accept(dictBuilder);

	char* argv[] = { "cdefcompiler" };
	lib_perf_counter::Configuration config((int)0, argv);

	bool failed = false;
	lib_perf_counter::ManifestGenerator manifestGenerator("ManifestGenerator01.cdef");
	BOOST_CHECK_MESSAGE(manifestGenerator.generate([&failed](const CompileMessage& message) {
		std::cout << "msg:" << message.getMessage() << std::endl;
		failed = true;
	},
	dictBuilder.getDictionary()), "manifestGenerator.generate failed");

	BOOST_CHECK_MESSAGE(failed == false, "Unexpected compile error");
}

BOOST_AUTO_TEST_SUITE_END()
