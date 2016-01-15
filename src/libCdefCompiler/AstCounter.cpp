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
#include <boost/algorithm/string.hpp>

#include "AstCounter.hpp"
#include "CompileMessage.hpp"

namespace lib_perf_counter {

bool AstCounter::parse(const pt::ptree& tree, MessageCallback callback)
{
	// get the name associated with this collection
	// todo: validate the name
	_label = tree.get_value<string>();

	// Loop through all properties on this level
	for (const pt::ptree::value_type &v : tree)
	{
		if (boost::iequals(v.first.data(), "id")) {
			if (!isValidUInt(tree.get<std::string>("id"))) {
				CompileMessage error(CompileMessage::type_t::Error, CdefMessage::CDE301, callback, "The counter '%s' contains an invalid id", _label.c_str());
				return false;
			}
			_id = tree.get<uint32_t>("id");
		}
		else if (boost::iequals(v.first.data(), "name"))
			_name = tree.get<std::string>("name");
		else if (boost::iequals(v.first.data(), "description"))
			_description = tree.get<std::string>("description");
		else if (boost::iequals(v.first.data(), "type"))
		{
			if (boost::iequals(v.second.data(), "Counter32"))
				_type = type_t::Counter32;
			else if (boost::iequals(v.second.data(), "Gauge32"))
				_type = type_t::Gauge32;
			else if (boost::iequals(v.second.data(), "Gauge64"))
				_type = type_t::Gauge64;
			else {
				CompileMessage error(CompileMessage::type_t::Error, CdefMessage::CDE302, callback, "Unknown counter type: '%s'", v.second.data().c_str());
				return false;
			}
		}
	}

	// perform a last chance sanity check on the parsed data
	return isSane(callback);
}

bool AstCounter::isSane(MessageCallback callback)
{
	if (0 == _label.length()) {
		ostringstream message;
		CompileMessage error(CompileMessage::type_t::Error, CdefMessage::CDE303, callback, "An unlabled counter was found");
		return false;
	}

	if (!isValidCname(_label)) {
		CompileMessage error(CompileMessage::type_t::Error, CdefMessage::CDE304, callback, "The counter label '%s' is not a valid CNAME", _label.c_str());
		return false;
	}
	

	if (_id == 0) {
		CompileMessage error(CompileMessage::type_t::Error, CdefMessage::CDE305, callback, "The id in counter '%s' is zero (0)", _label.c_str());
		return false;
	}

	if (0 == _name.length()) {
		CompileMessage error(CompileMessage::type_t::Error, CdefMessage::CDE306, callback, "The name in counter '%s' is missing", _label.c_str());
		return false;
	}

	if (0 == _description.length()) {
		CompileMessage error(CompileMessage::type_t::Error, CdefMessage::CDE307, callback, "The description in counter '%s' is missing", _label.c_str());
		return false;
	}

	if (_type >= type_t::undefined) {
		CompileMessage error(CompileMessage::type_t::Error, CdefMessage::CDE308, callback, "The type in counter '%s' is missing or invalid", _label.c_str());
		return false;
	}

	return true;
}

const string AstCounter::getTypeAsPerfCounterType() const
{
	switch (_type)
	{
		case type_t::Counter32:
			return "perf_counter_rawcount";
			break;
		case type_t::Gauge32:
			return "perf_raw_base";
			break;
		case type_t::Gauge64:
			return "perf_large_raw_base";
			break;
		default:
			assert(false && "Impossible value for AstCounter::Type encountered");
			return "";
	}
}

const string AstCounter::getTypeAsSnmpCounterType() const
{
	switch (_type)
	{
		case type_t::Counter32:
			return "Counter32";
			break;
		case type_t::Gauge32:
			return "Gauge32";
			break;
		case type_t::Gauge64:
			return "Gauge64";
			break;
		default:
			assert(false && "Impossible value for AstCounter::Type encountered");
			return "";
	}
}

}