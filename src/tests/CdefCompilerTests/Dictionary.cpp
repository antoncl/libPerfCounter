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

#include "../../libCdefCompiler/Compiler.hpp"
#include "../../libCdefCompiler/AstVisitor.hpp"
#include "../../libCdefCompiler/AstProvider.hpp"
#include "../../libCdefCompiler/DictionaryBuilder.hpp"
#include "../../externals/libTemplateEngine/libTemplateEngine/TemplateEngine.hpp"
#include "../../externals/libTemplateEngine/libTemplateEngine/DictionaryList.hpp"

// Boost test doesn't know these types and cannot output their value
BOOST_TEST_DONT_PRINT_LOG_VALUE(template_engine::te_string);

// Validate that the AST is working as expected
BOOST_AUTO_TEST_SUITE(Ast);

using namespace lib_perf_counter;
namespace te = ::template_engine;
namespace pt = ::boost::property_tree;


BOOST_AUTO_TEST_CASE( Dictionary )  
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

	bool foundError;
	AstProviderPtr root = AstProvider::create();
	root->parse(tree, [&foundError](const CompileMessage& message) {
		BOOST_ERROR("Unexpected compile error");
	});

	DictionaryBuilder builder;
	root->accept(builder);
	te::DictionaryPtr d = builder.getDictionary();

	// get every expected value from the dictionary.
	BOOST_CHECK(d->getValue(TE_TEXT("APPLICATION")) == TE_TEXT("test.exe"));
	BOOST_CHECK(d->getValue(TE_TEXT("UUID")) == TE_TEXT("4417944e-470d-488d-9265-122b84bfe192"));
	BOOST_CHECK(d->getValue(TE_TEXT("LABEL")) == TE_TEXT("test"));
	te::DictionaryListPtr dl(nullptr);
	BOOST_CHECK( (dl = d->getList(TE_TEXT("CounterSets"))) != nullptr);

	dl->resetCursor();
	d = dl->getCurrent();
	BOOST_CHECK(d->getValue(TE_TEXT("UUID")) == TE_TEXT("cadff40d-91d8-41b3-8a78-9fa587dbd137"));
	BOOST_CHECK(d->getValue(TE_TEXT("NAMESPACE")) == TE_TEXT("test.set1"));
	BOOST_CHECK(d->getValue(TE_TEXT("NAME")) == TE_TEXT(""));
	BOOST_CHECK(d->getValue(TE_TEXT("DESCRIPTION")) == TE_TEXT("some description"));
	BOOST_CHECK(d->getValue(TE_TEXT("LABEL")) == TE_TEXT("set1"));
	BOOST_CHECK((dl= d->getList(TE_TEXT("Counters"))) != nullptr);

	dl->resetCursor();
	d = dl->getCurrent();
	BOOST_CHECK(d->getValue(TE_TEXT("ID")) == TE_TEXT("1"));
	BOOST_CHECK(d->getValue(TE_TEXT("NAMESPACE")) == TE_TEXT("test.set1.c1"));
	BOOST_CHECK(d->getValue(TE_TEXT("NAME")) == TE_TEXT("name1"));
	BOOST_CHECK(d->getValue(TE_TEXT("DESCRIPTION")) == TE_TEXT("some other description"));
	BOOST_CHECK(d->getValue(TE_TEXT("LABEL")) == TE_TEXT("c1"));
	BOOST_CHECK(d->getValue(TE_TEXT("WINTYPE")) == TE_TEXT("perf_raw_base"));
}

BOOST_AUTO_TEST_SUITE_END()
