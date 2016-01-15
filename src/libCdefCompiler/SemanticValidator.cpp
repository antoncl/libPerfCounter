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
#include "SemanticValidator.hpp"
#include <boost/uuid/uuid_io.hpp>

#include "AstProvider.hpp"
#include "AstCounterSet.hpp"
#include "AstCounter.hpp"

namespace lib_perf_counter {

SemanticValidator::SemanticValidator(MessageCallback callback) :
	_callback(callback),
	_usedUuids(),
	_usedCountersetNames(),
	_usedCountersetLabels(),
	_usedCounterLabels(),
	_usedCounterNames(),
	_usedCounterIds(),
	_currentCounterSet(nullptr)
{
}

SemanticValidator::~SemanticValidator()
{
}

bool SemanticValidator::uuidIsUnique(const uuid id)
{
	UuidSet::const_iterator got = _usedUuids.find(id);

	if(got != _usedUuids.end())	{
		CompileMessage error(CompileMessage::type_t::Error, CdefMessage::CDE401, _callback, "The uuid '%s' is used more than once", to_string(id).c_str());
		return false;
	}

	_usedUuids.insert(id);
	return true;
}

bool SemanticValidator::nameIsUnique(StringSet set, const std::string name, CdefMessage msgType, const std::string& property)
{
	StringSet::const_iterator got = set.find(name);

	if (got != set.end()) {
		CompileMessage error(CompileMessage::type_t::Error, msgType, _callback, 
			"The %s '%s' is used more than once", property.c_str(), name.c_str());
		return false;
	}

	set.insert(name);
	return true;
}

bool SemanticValidator::intIdUnique(uint32_t id, const std::string& countersetName)
{
	IntegerSet::iterator got = _usedCounterIds.find(id);

	if (got != _usedCounterIds.end()) {
		CompileMessage error(CompileMessage::type_t::Error, CdefMessage::CDE404, _callback,
			"The id '%s' is used more than once in the counterset '%s'", to_string(id).c_str(), countersetName.c_str());
		return false;
	}

	_usedCounterIds.insert(id);
	return true;

}

void SemanticValidator::preVisit(AstProvider &provider)
{
	// the uuid must be globally unique
	if (!uuidIsUnique(provider.getUuid()))
		return;

	// start a new scope on counter sets
	_usedCountersetNames.clear();
	_usedCountersetLabels.clear();
}

void SemanticValidator::preVisit(AstCounterSet &counterset)
{
	// the uuid must be globally unique
	if (!uuidIsUnique(counterset.getUuid()))
		return;

	// the counterset name must be globally unique
	if (!nameIsUnique(_usedCountersetNames, counterset.getName(), CdefMessage::CDE402, "counterset name"))
		return;

	// the counterset label must be globally unique
	if (!nameIsUnique(_usedCountersetLabels, counterset.getLabel(), CdefMessage::CDE403, "counterset label"))
		return;

	// start a new scope on counters
	_usedCounterLabels.clear();
	_usedCounterNames.clear();
	_usedCounterIds.clear();
	_currentCounterSet = &counterset;
}

void SemanticValidator::visit(AstCounter &counter)
{
	// the label must be unique
	_usedCounterLabels.clear();

	// the name must be unique
	if (!nameIsUnique(_usedCounterNames, counter.getName(), CdefMessage::CDE405, "counter name"))
		return;

	// the id must be unique
	if (!nameIsUnique(_usedCounterLabels, counter.getLabel(), CdefMessage::CDE406, "counter label"))
		return;

	if (!intIdUnique(counter.getId(), _currentCounterSet ? _currentCounterSet->getLabel() : "<unknown>"))
		return;

}

}