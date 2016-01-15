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

#ifndef __ASTVISITOR_H_
#define __ASTVISITOR_H_

namespace lib_perf_counter {

class AstProvider;
class AstCounterSet;
class AstCounter;

/** Empty implementation of an ast visitor.  
 * Visitor implementations can inherit from this class and then  
 * simply override the methods they intend to use.
 */
class AstVisitor
{
public:

	/**
	 * Pre-order visit an AstProvider.
	 *
	 * \param provider The AstProvider being visited
	 */
	virtual void preVisit(AstProvider& /*provider*/) { }

	/**
	* Post-order visit an AstProvider.
	*
	* \param provider The AstProvider being visited
	*/
	virtual void postVisit(AstProvider& /*provider*/) { }

	/**
	* Pre-order visit an AstCounterSet.
	*
	* \param counterSet The AstCounterSet being visited
	*/
	virtual void preVisit(AstCounterSet& /*counterSet*/) { }

	/**
	* Post-order visit an AstCounterSet.
	*
	* \param counterSet The AstCounterSet being visited
	*/
	virtual void postVisit(AstCounterSet& /*counterSet*/) { }

	/**
	* Visit an AstCounter.
	*
	* \param counter The AstCounter being visited
	*/
	virtual void visit(AstCounter& /*Counter*/) { }
};

}
#endif // !__ASTVISITOR_H_
