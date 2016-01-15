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

#ifndef __SEMANTIC_VALIDATOR_HPP_
#define __SEMANTIC_VALIDATOR_HPP_

#include <string>
#include <unordered_set>
#include <boost/uuid/uuid.hpp>
#include <boost/functional/hash.hpp>

#include "CompileMessage.hpp"
#include "AstVisitor.hpp"


namespace lib_perf_counter {

using namespace boost::uuids;

class SemanticValidator : public AstVisitor
{
public:
	using AstVisitor::postVisit;
	using AstVisitor::preVisit;
	using AstVisitor::visit;

	SemanticValidator(MessageCallback callback);
	~SemanticValidator();

	virtual void visit(AstCounter&);

	virtual void preVisit(AstCounterSet&);

	virtual void preVisit(AstProvider&);

private:
	typedef std::unordered_set<boost::uuids::uuid, boost::hash<boost::uuids::uuid>> UuidSet;
	typedef std::unordered_set<std::string> StringSet;
	typedef std::unordered_set<uint32_t> IntegerSet;

	bool uuidIsUnique(const uuid id);
	bool nameIsUnique(StringSet set, const std::string name, CdefMessage msgType, const std::string& property);
	bool intIdUnique(uint32_t id, const std::string& counterName);

	MessageCallback _callback;


	UuidSet _usedUuids;
	StringSet _usedCountersetNames;
	StringSet _usedCountersetLabels;
	StringSet _usedCounterLabels;
	StringSet _usedCounterNames;
	IntegerSet _usedCounterIds;
	AstCounterSet* _currentCounterSet;
};

}
#endif // !__SEMANTIC_VALIDATOR_HPP_