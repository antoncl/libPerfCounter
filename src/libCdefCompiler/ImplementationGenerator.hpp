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

#ifndef __IMPLEMENTATION_GENERATOR_HPP_
#define __IMPLEMENTATION_GENERATOR_HPP_

#include <boost/filesystem.hpp>

#include <string>
#include <vector>

#include "../externals/libTemplateEngine/libTemplateEngine/TemplateEngine.hpp"
#include "CompileMessage.hpp"



namespace lib_perf_counter {
namespace fs = boost::filesystem;
namespace te = template_engine;

class ImplementationGenerator
{
public:

	/**
	* Constructor.
	*
	* \param	sourceFileName	Filename of the source file.
	*/
	ImplementationGenerator(const fs::path& sourceFileName);

	/**
	* Generate the C++ Counter implementation using the given dictionary set.
	*
	* \param	cdefRoot	The dictionary to use when expanding template elements.
	* \return true if the generation succeeded, false otherwise
	*/
	bool generate(MessageCallback callback, te::DictionaryPtr cdefRoot);
private:
	fs::path _sourceFileName;				///< The counter definition source file to compile.
	const static std::u16string _template;	///< template for the XML manifest file.

};

}
#endif // !__IMPLEMENTATION_GENERATOR_HPP_
