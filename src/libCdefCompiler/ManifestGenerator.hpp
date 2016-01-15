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

#ifndef __GENERATEMANIFEST_HPP_
#define __GENERATEMANIFEST_HPP_

#include <boost/filesystem.hpp>

#include <string>
#include <vector>

#include "../externals/libTemplateEngine/libTemplateEngine/TemplateEngine.hpp"
#include "AstVisitor.hpp"
#include "CompileMessage.hpp"

namespace lib_perf_counter {
namespace fs = boost::filesystem;
namespace te = template_engine;

/** The XML manifest generator.  
 * Windows performance counters are defined in an XML manifest file, which is then
 * converted into an .h header file and a .rc file by the ctrpp.exe tool.
 * 
 * The header contains c functions which can/should be used to start the windows API's
 * monitoring. The .rc file contains text strings which will be used to provide
 * locale specific text strings about the counters.
 */
class ManifestGenerator
{
private:
	fs::path _sourceFileName;				///< The counter definition source file to compile.
	const static std::u16string _template;	///< template for the XML manifest file.

public:

	/**
	 * Constructor.
	 *
	 * \param	sourceFileName	Filename of the source file.
	 */
	ManifestGenerator(const fs::path& sourceFileName);

	/**
	 * Generate the XML manifest file using the given dictionary set.
	 *
	 * \param	cdefRoot	The dictionary to use when expanding template elements.
	 * \return true if the generation succeeded, false otherwise
	 */
	bool generate(MessageCallback callback, te::DictionaryPtr cdefRoot);
};

}
#endif // !__GENERATEMANIFEST_HPP_