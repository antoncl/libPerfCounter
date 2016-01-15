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

#ifndef __ASTCOUNTER_HPP_
#define __ASTCOUNTER_HPP_

#include <string>
#include <iostream>
#include <boost/property_tree/ptree.hpp>

#include "AstNode.hpp"
#include "AstVisitor.hpp"
#include "AstCounterSet.hpp"

namespace lib_perf_counter {

using namespace std;
namespace pt = boost::property_tree;

/** An ast counter. */
class AstCounter : AstNode
{
public:
	/** List the different kinds of counters available. */
	enum class type_t
	{
		Counter32,  ///< 32 bit ever increasing value.
		Gauge32,	///< 32 bit value, which may go up or down.
		Gauge64,	///< 64 bit value, which may go up or down.
		undefined   ///< Reperesents any invalid couter type.
	};

	/** \copydoc AstNode::AstNode */
	AstCounter() :
		_label(""),
		_id(0),
		_name(""), 
		_description(""), 
		_type(type_t::undefined),
		_parent()
	{ };

	/** \copydoc AstNode::compile */
	virtual bool parse(const pt::ptree& tree, MessageCallback callback);

	/** \copydoc AstNode::isSane */
	virtual bool isSane(MessageCallback errors);

	/** Get type name as defined by windows. */
	const string getTypeAsPerfCounterType() const;

	/** Get type name as defined by SNMP. */
	const string getTypeAsSnmpCounterType() const;

	/** Get label of this counter. */
	inline const string& getLabel() const { return _label; }

	/** Get id of this counter. */
	inline const uint32_t getId() const { return _id; }

	/** Get name of this counter. */
	inline const string& getName() const { return _name; }
	
	/** Get description of this counter. */
	inline const string& getDescription() const { return _description; }

	/** Get type of this counter. */
	inline const type_t getType() const { return _type; }

	inline virtual void accept(AstVisitor& visitor)
	{
		visitor.visit(*this);
	}

	/** Get the parent of this counter set. */
	inline const AstCounterSetPtr getParent() const { return _parent.lock(); }

	/** Set the parent of this counter set. */
	inline const void setParent(const AstCounterSetPtr& parent) { _parent = parent; }

	/** Gets the namespace of this counter. */
	inline const string getNamespace() const
	{ 
		AstCounterSetPtr _realParent = _parent.lock();
		assert(_realParent && "Parent has dissapeared");
		return _realParent ? _realParent->getNamespace() + "." + _label : _label;
	}

private:
	string _label;				///< The label/cname of this counter.
	uint32_t _id;				///< The integer identifier of this counter.
	string _name;				///< The descriptive name of this counter.
	string _description;		///< The description of this counter.
	type_t	_type;				///< The performance counter type of this counter.
	std::weak_ptr<AstCounterSet> _parent;	///< The counter set this counter belongs to.
};

}
#endif//__ASTCOUNTER_HPP_