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
#include <string>
#include <set>
#include <exception>
#include <iostream>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/info_parser.hpp>

#include "Compiler.hpp"
#include "AstProvider.hpp"
#include "SemanticValidator.hpp"
#include "DictionaryBuilder.hpp"
#include "ManifestGenerator.hpp"
#include "ImplementationGenerator.hpp"

namespace lib_perf_counter {

Compiler::Compiler(const fs::path& sourceFileName) :
	_sourceFileName(sourceFileName)
{
}

void Compiler::errorHandler(const CompileMessage& message)
{
	cerr << _sourceFileName << ':' << message << endl;
}

bool Compiler::Run()
{
	pt::ptree tree;
	// Parse the definitions file into the property tree.
	pt::read_info(_sourceFileName.string(), tree);

	AstProviderPtr ast = AstProvider::create();

	// convert the input into an AST
	bool result = ast->parse(tree, std::bind(&Compiler::errorHandler, this, std::placeholders::_1));
	if (!result)
		return false;

	// prevent naming conflicts, etc.
	SemanticValidator validator(std::bind(&Compiler::errorHandler, this, std::placeholders::_1));
	ast->accept(validator);

	// build a dictionary set of all defined properties
	DictionaryBuilder dictBuilder;
	ast->accept(dictBuilder);

	// generate the XML manifest for the Windows based performance counters
	ManifestGenerator manifestGenerator(_sourceFileName);
	result = manifestGenerator.generate(std::bind(&Compiler::errorHandler, this, std::placeholders::_1), dictBuilder.getDictionary());
	if (!result)
		return false;

	// generate the XML manifest for the Windows based performance counters
	ImplementationGenerator implementationGenerator(_sourceFileName);
	result = implementationGenerator.generate(std::bind(&Compiler::errorHandler, this, std::placeholders::_1), dictBuilder.getDictionary());
	if (!result)
		return false;

	return true;
}

}