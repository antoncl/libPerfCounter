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
#include <cstdarg>
#include <memory>
#include <exception>
#include <stdexcept>
#include "CompileMessage.hpp"

namespace lib_perf_counter {
	
CompileMessage::CompileMessage(type_t type, uint32_t id, MessageCallback callback, const char* fmt, ...) :
	_type(type),
	_id(id),
	_message()
{
	va_list ap;
	va_start(ap, fmt);
	_message = vformat(fmt, ap);
	va_end(ap);

	if(callback)
		callback(*this);
}

std::string CompileMessage::vformat(const char *fmt, va_list ap)
{
	// Allocate a buffer on the stack that's big enough for us almost
	// all the time.  Be prepared to allocate dynamically if it doesn't fit.
	size_t length = 256;	// in characters
	char stackbuf[256 * sizeof(char)];
	char *buf = &stackbuf[0];
	std::shared_ptr<char> heapbuf(nullptr);

	while (1) {
		// Try to vsnprintf into our buffer.
		int needed = vsnprintf(buf, length, fmt, ap);
		// NB. C99 (which modern Linux and OS X follow) says vsnprintf
		// failure returns the length it would have needed.  But older
		// glibc and current Windows return -1 for failure, i.e., not
		// telling us how much was needed.

		if (needed <= (int)length && needed >= 0) {
			// It fit fine so we're done.
			return std::string(buf, (size_t)needed);
		}

		// vsnprintf reported that it wanted to write more characters
		// than we allotted.  So try again using a dynamic buffer.  This
		// doesn't happen very often if we chose our initial size well.
		length = (needed > 0) ? (needed + 1) : (length * 2);
		if (length > 65536)
			throw std::length_error("print buffer exceeds 64k");

		heapbuf.reset(new char[length * sizeof(char)]);
		buf = heapbuf.get();
	}
}

std::ostream& operator<<(std::ostream& os, const CompileMessage& message)
{
	switch (message.getType())
	{
	case CompileMessage::type_t::Error:
		os << "Error";
		break;
	case CompileMessage::type_t::Warn:
		os << "Warning";
		break;
	case CompileMessage::type_t::Info:
		os << "Info";
		break;
	}
	os << ' ' << "CDE" << message.getId() << ':' << message.getMessage();
	return os;
}

}