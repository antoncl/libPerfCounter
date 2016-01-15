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

#ifndef __ASTCOUNTERSET_HPP_
#define __ASTCOUNTERSET_HPP_

#include <string>
#include <list>
#include <iostream>

#include <boost/property_tree/ptree.hpp>
#include <boost/uuid/uuid.hpp>
#include <boost/uuid/uuid_io.hpp>
#include <boost/uuid/nil_generator.hpp>

#include "AstNode.hpp"
#include "AstVisitor.hpp"
#include "AstProvider.hpp"

namespace lib_perf_counter {

using namespace std;
using namespace boost::uuids;

namespace pt = boost::property_tree;

class AstCounter;
typedef shared_ptr<AstCounter> AstCounterPtr;

class AstCounterSet;
typedef shared_ptr<AstCounterSet> AstCounterSetPtr;

/** An ast counter set. */
class AstCounterSet : public enable_shared_from_this<AstCounterSet>, public list<AstCounterPtr>, public AstNode
{
private:
	string	_label;				///< The label
	string	_description;		///< The description
	string	_name;				///< The name
	uuid	_uuid;				///< The uuid
	std::weak_ptr<AstProvider> _parent;		///< The provider this counter set belongs to 

	/** \copydoc AstNode::AstNode */
	AstCounterSet();
public:
	static AstCounterSetPtr create()
	{
		return std::shared_ptr<AstCounterSet>(new AstCounterSet());
	}

	/** \copydoc AstNode::parse */
	virtual bool parse(const pt::ptree& tree, MessageCallback callback);

	/** \copydoc AstNode::isSane */
	bool isSane(MessageCallback callback);

	/** \copydoc AstNode::Accept */
	virtual void accept(AstVisitor& visitor);

	/** Get the label. */
	inline const string getLabel() const { return _label; }

	/** Get the name. */
	inline const string getName() const { return _name; }

	/** Get the description. */
	inline const string getDescription() const { return _description; }

	/** Get the uuid. */
	inline const uuid getUuid() const { return _uuid; }

	/** Get the parent of this counter set. */
	inline const AstProviderPtr getParent() const { return _parent.lock(); }

	/** Set the parent of this counter set. */
	inline const void setParent(const AstProviderPtr& parent)
	{
		_parent = parent;
	}

	/** Gets the namespace of this counter set. */
	inline const string getNamespace() const
	{
		AstProviderPtr _realParent = _parent.lock();
		assert(_realParent && "Parent has dissapeared");
		return _realParent ? _realParent->getNamespace() + "." + _label : _label;
	}
};

}
#endif // !__ASTCOUNTERSET_HPP_
