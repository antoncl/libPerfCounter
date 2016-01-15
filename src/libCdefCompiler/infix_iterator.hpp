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

// infix_iterator.h 
// 
// Lifted from Jerry Coffin's 's prefix_ostream_iterator 
#ifndef __INFIX_ITERATOR_H_
#define __INFIX_ITERATOR_H_ 

#include <ostream> 
#include <iterator> 

namespace lib_perf_counter {

template <class T, class charT = char, class traits = std::char_traits<charT> >
class infix_ostream_iterator :
	public std::iterator<std::output_iterator_tag, void, void, void, void>
{
	std::basic_ostream<charT, traits> *os;
	charT const* delimiter;
	bool first_elem;
public:

	typedef charT char_type;
	typedef traits traits_type;
	typedef std::basic_ostream<charT, traits> ostream_type;

	infix_ostream_iterator(ostream_type& s)
		: os(&s), delimiter(0), first_elem(true)
	{}

	infix_ostream_iterator(ostream_type& s, charT const *d)
		: os(&s), delimiter(d), first_elem(true)
	{}

	infix_ostream_iterator<T, charT, traits>& operator=(T const &item)
	{
		// Here's the only real change from ostream_iterator: 
		// Normally, the '*os << item;' would come before the 'if'. 
		if (!first_elem && delimiter != 0)
			*os << delimiter;
		*os << item;
		first_elem = false;
		return *this;
	}

	infix_ostream_iterator<T, charT, traits> &operator*() {
		return *this;
	}

	infix_ostream_iterator<T, charT, traits> &operator++() {
		return *this;
	}

	infix_ostream_iterator<T, charT, traits> &operator++(int) {
		return *this;
	}
};

}
#endif//__INFIX_ITERATOR_H_