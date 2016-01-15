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

#ifndef __AST_PROVIDER_HPP_
#define __AST_PROVIDER_HPP_

#include <string>
#include <list>

#include <boost/uuid/uuid.hpp>
#include <boost/uuid/uuid_io.hpp>
#include <boost/property_tree/ptree.hpp>

#include "CompileMessage.hpp"
#include "AstNode.hpp"
#include "AstVisitor.hpp"

namespace lib_perf_counter {

using namespace std;
using namespace boost::uuids;
namespace pt = ::boost::property_tree;

class AstCounterSet;
typedef shared_ptr<AstCounterSet> AstCounterSetPtr;

class AstProvider;
typedef shared_ptr<AstProvider> AstProviderPtr;
/** AST element for the provider block. */
class AstProvider : public enable_shared_from_this<AstProvider>, public list<AstCounterSetPtr>, public AstNode
{
private:
	string _label;			///< The c code name of this provider
	string _application;	///< The application name this library provides counters for
	uuid   _uuid;			///< The uuid

	/** Default constructor. */
	AstProvider();
public:
	static AstProviderPtr create()
	{
		return std::shared_ptr<AstProvider>(new AstProvider());
	}

	/**
	 * Constructor for the Provider AST node
	 *
	 * \param	tree		  	The property tree to extract values from.
	 * \param [in,out]	errors	Any errors are reported on this stream.
	 */
	 /** \copydoc AstNode::generate */
	virtual bool parse(const pt::ptree& tree, MessageCallback callback);

	/** \copydoc AstNode::isSane */
	virtual bool isSane(MessageCallback errors);

	/** Gets the label. */
	inline const string getLabel() const { return _label; }

	/** Gets the application name. */
	inline const string getApplication() const { return _application; }

	/** Gets the uuid of this provider. */
	inline const uuid getUuid() const { return _uuid; }

	/** Gets the namespace of this provider. */
	inline const string getNamespace() const { return _label; }

	/** \copydoc AstNode::Accept */
	inline virtual void accept(AstVisitor& visitor);
};

}
#endif // ! __AST_PROVIDER_HPP_
