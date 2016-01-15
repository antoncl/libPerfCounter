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

#ifndef __CDEF_COMPILER_TARGETVER_H_
#define __CDEF_COMPILER_TARGETVER_H_

#if defined(_WIN32)

#include <WinSDKVer.h>
#define _WIN32_WINNT	_WIN32_WINNT_VISTA
#define WINVER			_WIN32_WINNT_VISTA
#endif

#endif // !__CDEF_COMPILER_TARGETVER_H_

