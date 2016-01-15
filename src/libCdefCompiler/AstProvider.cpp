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
#include <iostream>
#include <boost/optional.hpp>
#include <boost/uuid/uuid_generators.hpp>
#include <boost/algorithm/string.hpp>

#include "AstProvider.hpp"
#include "AstCounterSet.hpp"

namespace lib_perf_counter {

namespace pt = boost::property_tree;

AstProvider::AstProvider() :
	_label(),
	_application(),
	_uuid()
{}

bool AstProvider::parse(const ::boost::property_tree::ptree& tree, MessageCallback callback)
{
	boost::optional< const pt::ptree& > child = tree.get_child_optional("provider");
	if (!child) {
		CompileMessage error(CompileMessage::type_t::Error, CdefMessage::CDE101, callback, "Provider block not defined");
		return false;
	}

	// get the name associated with this collection
	_label = child.get().get_value<string>();

	// Loop through all properties on this level
	for (const pt::ptree::value_type &v : child.get())
	{
		if (boost::iequals(v.first.data(), "application"))
			_application = child.get().get<string>("application");
		else if (boost::iequals(v.first.data(), "uuid")) {
			// read and Validate the uuid
			string uuid = v.second.data();
			if (!isValidUuid(uuid)) {
				CompileMessage error(CompileMessage::type_t::Error, CdefMessage::CDE102, callback, "The provider '%s' contains an invalid uuid", _label.c_str());
				return false;
			}
			_uuid = string_generator()(uuid);
		}
		else if (boost::iequals(v.first.data(), "counterset")) {
			AstCounterSetPtr counterset = AstCounterSet::create();
			if(!counterset->parse(v.second, callback))
				return false;
			
			counterset->setParent(shared_from_this());

			// the contructor will throw an exception if a syntax error was detected.
			this->push_back(counterset);
		}
	}

	return isSane(callback);
}

bool AstProvider::isSane(MessageCallback callback)
{
	// Providers must have a label
	if (0 == _label.length()) {
		CompileMessage error(CompileMessage::type_t::Error, CdefMessage::CDE102, callback, "The provider is unlabled");
		return false;
	}

	// the label must be a valid name for a c++ struct
	if (!isValidCname(_label)) {
		CompileMessage error(CompileMessage::type_t::Error, CdefMessage::CDE103, callback, "The provider label '%s' is not a valid CNAME", _label.c_str());
		return false;
	}

	//TODO: Should check against valid filenames, and .exe on windows platforms
	static const std::regex appLabel("[_a-zA-Z][_0-9a-zA-Z]*\\.[eE][xX][eX]");
	if (!std::regex_match(_application, appLabel)) {
		CompileMessage error(CompileMessage::type_t::Error, CdefMessage::CDE104, callback, "The application name in provider '%s' is invalid", _label.c_str());
		return false;
	}

	// the uuid cannot be null
	if (_uuid.is_nil()) {
		CompileMessage error(CompileMessage::type_t::Error, CdefMessage::CDE105, callback, "The provider '%s' contains a null uuid", _label.c_str());
		return false;
	}

	if (0 == this->size()) {
		CompileMessage error(CompileMessage::type_t::Error, CdefMessage::CDE106, callback, "The provider '%s' does not define any countersets", _label.c_str());
		return false;
	}
	return true;
}

void AstProvider::accept(AstVisitor& visitor)
{
	visitor.preVisit(*this);

	for (AstCounterSetPtr n : *this)
		n->accept(visitor);

	visitor.postVisit(*this);
}

}