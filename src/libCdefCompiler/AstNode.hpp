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

#ifndef __ASTNODE_HPP_
#define __ASTNODE_HPP_

#include <ostream>
#include <regex>
#include <functional>
#include <boost/property_tree/ptree.hpp>

#include "CompileMessage.hpp"

namespace lib_perf_counter {

namespace pt = ::boost::property_tree;

class AstVisitor;

/** Abstract base class for AST nodes */
class AstNode
{
public:

	/** Default constructor. */
	AstNode() {};

	// no copying is permitted
	AstNode(const AstNode&) = delete;
	AstNode& operator=(const AstNode&) = delete;
	AstNode(AstNode&&) = delete;
	AstNode& operator=(AstNode&&) = delete;

	/**
	 * Generate the node from the given property tree.
	 *
	 * \param	tree		  	The property tree to retrieve property values from.
	 * \param [in,out]	errors	Any errors in the properties are reported on this stream.
	 */
	virtual bool parse(const pt::ptree& /*tree*/, MessageCallback /*callback*/) = 0;

	/**
	 * Query if this node when seen in isolation is sane, e.g. no data is missing and nothing is out of range
	 *
	 * \param [in,out]	errors	The function to callback with any errors.
	 *
	 * \return	true if the node seems to be in a sane state , false if not.
	 */
	virtual bool isSane(MessageCallback) = 0;

	/**
	* Implement the visitor pattern.
	*
	* \param	visitor	The visitor to accept.
	* \seealso  AstVisitor
	*/
	virtual void accept(AstVisitor& visitor) = 0;

	/**
	 * Query if 'uuid' is valid uuid.
	 * 
	 * \param	uuidAsString	The uuid to examine.
	 *
	 * \return	true if valid uuid, false if not.
	 */
	inline bool isValidUuid(std::string uuidAsString)
	{
		static const std::regex e("[a-fA-F0-9]{8}-[a-fA-F0-9]{4}-4[a-fA-F0-9]{3}-[89aAbB][a-fA-F0-9]{3}-[a-fA-F0-9]{12}");
		return std::regex_match(uuidAsString, e); 
	}

	/**
	 * Query if 'intAsString' is valid u int.
	 *
	 * \param	intAsString	The int as string.
	 *
	 * \return	true if valid u int, false if not.
	 */
	inline bool isValidUInt(std::string intAsString)
	{
		static const std::regex e("[0-9]{1,10}");
		return std::regex_match(intAsString, e); 
	}

	/**
	 * Query if 'cnameString' is valid cname.
	 *
	 * \param	cnameString	String to be validated.
	 *
	 * \return	true if valid cname, false if not.
	 */

	inline bool isValidCname(std::string cnameString)
	{
		static const std::regex e("[0-9a-zA-Z_]+");
		return std::regex_match(cnameString, e);
	}

};

}
#endif // !__ASTNODE_HPP_
