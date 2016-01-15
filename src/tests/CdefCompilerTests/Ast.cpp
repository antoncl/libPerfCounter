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
#include <boost/test/unit_test.hpp>
namespace utf = boost::unit_test;

#include "../../libCdefCompiler/Compiler.hpp"
#include "../../libCdefCompiler/AstVisitor.hpp"
#include "../../libCdefCompiler/AstProvider.hpp"
#include "../../libCdefCompiler/AstCounterSet.hpp"
#include "../../libCdefCompiler/AstCounter.hpp"

// Validate that the AST is working as expected
BOOST_AUTO_TEST_SUITE(Ast);

using namespace lib_perf_counter;
namespace pt = ::boost::property_tree;

BOOST_AUTO_TEST_CASE(Ast01)
{
	class AstVisitLogger : public AstVisitor
	{
	public:
		std::string _visitLog;

		AstVisitLogger() : _visitLog() {}

		void log(const std::string& message)
		{
			if (_visitLog.size() > 0)
				_visitLog.append("-");

			_visitLog += message;
		}

		const std::string& getLog() { return _visitLog; }

		virtual void visit(AstCounter& counter)
		{
			log("C");
		}

		virtual void preVisit(AstCounterSet& counterSet)
		{
			log("preCS");
		}

		virtual void postVisit(AstCounterSet& counterSet)
		{
			log("pstCS");
		}

		virtual void preVisit(AstProvider& provider)
		{
			log("preP");
		}

		virtual void postVisit(AstProvider& provider)
		{
			log("pstP");
		}
	};

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

	AstVisitLogger logger;
	root->accept(logger);
	BOOST_CHECK("preP-preCS-C-pstCS-pstP" == logger.getLog());
}

BOOST_AUTO_TEST_CASE(Ast02)
{
	class NameSpaceChecker : public AstVisitor
	{
	public:
		void preVisit(AstCounterSet& counterSet)
		{
			counterSetNamespace = counterSet.getNamespace();
		}

		void visit(AstCounter& counter)
		{
			counterNamespace = counter.getNamespace();
		}
		string counterSetNamespace;
		string counterNamespace;
	};

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

	NameSpaceChecker namespaceChecker;
	root->accept(namespaceChecker);
	BOOST_CHECK(namespaceChecker.counterSetNamespace == "test.set1");
	BOOST_CHECK(namespaceChecker.counterNamespace == "test.set1.c1");
}

BOOST_AUTO_TEST_SUITE_END()
