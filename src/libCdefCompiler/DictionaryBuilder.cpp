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
#include <boost/uuid/uuid.hpp>
#include <boost/uuid/uuid_io.hpp>
#include "DictionaryBuilder.hpp"

#include "AstProvider.hpp"
#include "AstCounterSet.hpp"
#include "AstCounter.hpp"

namespace lib_perf_counter {

DictionaryBuilder::DictionaryBuilder() :
	_providerDictionary(nullptr),
	_counterSetDictionary(nullptr),
	_counterDictionary(nullptr),
	_converter()
{
}


DictionaryBuilder::~DictionaryBuilder()
{
}

void DictionaryBuilder::preVisit(AstProvider& provider)
{
	// start a new scope of counter sets;
	_counterSetDictionary = std::make_shared<te::DictionaryList>();

	_providerDictionary = std::make_shared<te::Dictionary>();
	_providerDictionary->add(TE_TEXT("APPLICATION"), _converter.from_bytes(provider.getApplication()));
	_providerDictionary->add(TE_TEXT("UUID"), _converter.from_bytes(to_string(provider.getUuid())));
	_providerDictionary->add(TE_TEXT("LABEL"), _converter.from_bytes(provider.getLabel()));
	_providerDictionary->add(TE_TEXT("CounterSets"), _counterSetDictionary);
}

void DictionaryBuilder::preVisit(AstCounterSet& counterSet)
{
	// start a new scope of counters
	_counterDictionary = std::make_shared<te::DictionaryList>();

	te::DictionaryPtr d = std::make_shared<te::Dictionary>();
	d->add(TE_TEXT("UUID"), _converter.from_bytes(to_string(counterSet.getUuid())));
	d->add(TE_TEXT("NAMESPACE"), _converter.from_bytes(counterSet.getNamespace()));
	d->add(TE_TEXT("NAME"), _converter.from_bytes(counterSet.getName()));
	d->add(TE_TEXT("DESCRIPTION"), _converter.from_bytes(counterSet.getDescription()));
	d->add(TE_TEXT("LABEL"), _converter.from_bytes(counterSet.getLabel()));
	d->add(TE_TEXT("Counters"), _counterDictionary);

	_counterSetDictionary->add(d);
}

void DictionaryBuilder::visit(AstCounter& counter)
{
	te::DictionaryPtr d = std::make_shared<te::Dictionary>();
	d->add(TE_TEXT("ID"), _converter.from_bytes(std::to_string(counter.getId())));
	d->add(TE_TEXT("NAMESPACE"), _converter.from_bytes(counter.getNamespace()));
	d->add(TE_TEXT("NAME"), _converter.from_bytes(counter.getName()));
	d->add(TE_TEXT("DESCRIPTION"), _converter.from_bytes(counter.getDescription()));
	d->add(TE_TEXT("LABEL"), _converter.from_bytes(counter.getLabel()));
	d->add(TE_TEXT("WIN-TYPE"), _converter.from_bytes(counter.getTypeAsPerfCounterType()));
	d->add(TE_TEXT("SNMP-TYPE"), _converter.from_bytes(counter.getTypeAsSnmpCounterType()));

	_counterDictionary->add(d);
}

}