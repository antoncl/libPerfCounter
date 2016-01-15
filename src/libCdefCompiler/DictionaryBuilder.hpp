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

#ifndef __DICTIONARY_BUILDER_HPP_
#define __DICTIONARY_BUILDER_HPP_


#include "AstVisitor.hpp"

#include "../externals/libTemplateEngine/libTemplateEngine/TemplateEngine.hpp"

namespace lib_perf_counter {

namespace te = template_engine;

/** Build a hierarchy of dictionaries.    
 *  Another way of looking at this is that we convert a well defined hierarchy of typed
 *  objects into a hierarchy of dictionaries, where the type information is lost.
 */
class DictionaryBuilder : public AstVisitor
{
public:
	DictionaryBuilder();
	~DictionaryBuilder();

	/** \copydoc AstVisitor::preVisit(AstProvider&) */
	virtual void preVisit(AstProvider&);

	/** \copydoc AstVisitor::preVisit(AstCounterSet&) */
	virtual void preVisit(AstCounterSet&);

	/** \copydoc AstVisitor::visit(AstCounter&) */
	virtual void visit(AstCounter&);

	/**
	 * Gets the resulting Dictionary.
	 *
	 * \return	The dictionary.
	 */
	inline const te::DictionaryPtr getDictionary() { return _providerDictionary; }

protected:

private:
	te::te_converter _converter;					///< Converter between UTF-8 and UTF-16
	te::DictionaryPtr _providerDictionary;			///< root/Provider dictionary

	te::DictionaryListPtr _counterSetDictionary;	///< Temporary dictionary, unsed in the counter set scope
	te::DictionaryListPtr _counterDictionary;		///< Temporary dictionary, unsed in the counter scope
};

}
#endif // !__DICTIONARY_BUILDER_HPP_