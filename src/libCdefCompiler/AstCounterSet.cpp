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
#include <iostream>
#include <exception>
#include <boost/optional.hpp>
#include <boost/uuid/uuid_generators.hpp>
#include <boost/algorithm/string.hpp>

#include "CompileMessage.hpp"
#include "AstCounterSet.hpp"
#include "AstCounter.hpp"

namespace lib_perf_counter {

AstCounterSet::AstCounterSet() :
	_label(),
	_description(),
	_name(),
	_uuid(boost::uuids::nil_uuid()),
	_parent()
{
}

bool AstCounterSet::parse(const pt::ptree& tree, MessageCallback callback)
{
	// get the name associated with this collection
	// todo: validate the name
	_label = tree.get_value<std::string>();

	// Loop through all properties on this level
	for (const pt::ptree::value_type &v : tree)
	{
		if (boost::iequals(v.first.data(), "name"))
			_name = tree.get<std::string>("name");
		if (boost::iequals(v.first.data(), "description"))
			_description = tree.get<std::string>("description");
		else if (boost::iequals(v.first.data(), "uuid")) {
			// read and Validate the uuid
			string uuid = tree.get<string>("uuid");
			if (!isValidUuid(uuid)) {
				CompileMessage error(CompileMessage::type_t::Error, CdefMessage::CDE201, callback, "The counterset '%s' contains an invalid uuid", _label.c_str());
				return false;
			}
			_uuid = string_generator()(uuid);
		}
		else if (boost::iequals(v.first.data(), "counter")) {
			shared_ptr<AstCounter> counter = std::make_shared<AstCounter>();
			if (!counter->parse(v.second, callback))
				return false;

			counter->setParent(shared_from_this());

			this->push_back(counter);
		}
	}

	// perform a last stand sanity check
	return isSane(callback);
}

bool AstCounterSet::isSane(MessageCallback callback)
{
	if (0 == _label.length()) {
		CompileMessage error(CompileMessage::type_t::Error, CdefMessage::CDE202, callback, "An unlabled counterset was detected");
		return false;
	}

	if (!isValidCname(_label)) {
		CompileMessage error(CompileMessage::type_t::Error, CdefMessage::CDE203, callback, "The counterset label '%s' is not a valid CNAME", _label.c_str());
		return false;
	}

	if (0 == _description.length()) {
		CompileMessage error(CompileMessage::type_t::Error, CdefMessage::CDE204, callback, "The description in counter '%s' is missing", _label.c_str());
		return false;
	}

	if (_uuid.is_nil()) {
		CompileMessage error(CompileMessage::type_t::Error, CdefMessage::CDE205, callback, "The counterset '%s' contains a null uuid", _label.c_str());
		return false;
	}

	if (0 == this->size()) {
		CompileMessage error(CompileMessage::type_t::Error, CdefMessage::CDE206, callback, "the counterset '%s' is empty", _label.c_str());
		return false;
	}
	return true;
}

void AstCounterSet::accept(AstVisitor& visitor)
{
	visitor.preVisit(*this);

	for (shared_ptr<AstCounter> n : *this)
		n->accept(visitor);

	visitor.postVisit(*this);
}
}