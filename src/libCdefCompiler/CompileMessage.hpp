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

#ifndef __COMPILE_MESSAGE_HPP_
#define __COMPILE_MESSAGE_HPP_

#include <ostream>
#include <string>
#include <functional>

namespace lib_perf_counter {

/** Values that represent cdef errors.  
	* Used internally to keep track of which error numbers have been defined
	*/
enum CdefMessage
{
	ProviderMessage = 100,
	CDE101, CDE102, CDE103, CDE104, CDE105, CDE106,
	CounterSetMessage = 200,
	CDE201, CDE202, CDE203, CDE204, CDE205, CDE206,
	CounterMessage = 300,
	CDE301, CDE302, CDE303, CDE304, CDE305, CDE306, CDE307, CDE308,
	SemanticMessage = 400,
	CDE401, CDE402, CDE403, CDE404, CDE405, CDE406,
	GeneratorMessage = 500,
	CDE501, CDE502, CDE503, CDE504
};

class CompileMessage;

/** Defines the callback used when reporting an messages. */
typedef std::function<void(const CompileMessage&)> MessageCallback;

class CompileMessage
{
public:
	/** Values that represent the type of errors. */
	enum class type_t {
		Info,   ///< the message is not an error, it represents some information option
		Warn,   ///< the message is really only a warning
		Error,  ///< the message is really an error
	};

	/**
	 * Construct a compile message message
	 *
	 * \param	type		Is it informational, a warning or an error
	 * \param	id			The numeric message identifier.
	 * \param	callback	The callback to report the error message to.
	 * \param	fmt			The printf like format to use when formatting the message.
	 * \param	...			Variable arguments providing additional information.
	 */
	CompileMessage(type_t type, uint32_t id, MessageCallback callback, const char* fmt, ...);

	/** Get the formatted message. */
	inline const std::string& getMessage() const
	{
		return _message;
	}

	/** Get the numeric message identifier. */
	inline uint32_t getId() const
	{
		return _id;
	}

	/** Get the type of message, information, warning or error. */
	inline type_t getType() const
	{
		return _type;
	}
private:

	/**
	 * Helper function to safely call vsprintf.
	 *
	 * \param	fmt	The printf like format to use.
	 * \param	ap 	The start of the variable argument list (va_list).
	 *
	 * \return	A std::string.
	 * \throw	std::length_error if the formatted message exceeds 64k characters
	 */

	std::string vformat(const char *fmt, va_list ap);

	type_t _type;
	uint32_t _id;
	std::string _message;
};

/**
 * Pretty print the message onto the given stream.
 *
 * \param [in,out]	os	The stream to output onto.
 * \param	message   	The message to format.
 *
 * \return	The stream which was outputted on.
 */
std::ostream& operator<<(std::ostream& os, const CompileMessage& message);

}
#endif // !__COMPILE_MESSAGE_HPP_