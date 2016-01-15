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

#ifndef __COUNTERSIMPL_HPP_
#define __COUNTERSIMPL_HPP_

#if defined(_WIN32)
#	ifndef _WIN32_WINNT
#		include <SDKDDKVer.h>
#		define _WIN32_WINNT _WIN32_WINNT_WINXP
#	endif // !_WIN32_WINNT

#	define WIN32_LEAN_AND_MEAN             // Exclude rarely-used stuff from Windows headers
#	include <windows.h>
#	include <lmerr.h>
#	include "Counters.hpp"
#endif

#include <string>
#include <sstream>
#include <codecvt>
#include <memory>

class AbstractCounter
{
protected:
	AbstractCounter(PPERF_COUNTERSET_INSTANCE instance, ULONG id, PVOID counter)
	{
		ULONG status = PerfSetCounterRefValue(Yardmaster, instance, id, counter);
		if (status != ERROR_SUCCESS) {
			std::stringstream errorMessage;
			errorMessage << "Error setting reference to counter '" << id << "' " << displayErrorText(status) << "'";
			throw std::exception(errorMessage.str().c_str());
		}
	}

	~AbstractCounter()
	{
	}

	std::string displayErrorText(DWORD dwLastError)
	{
		std::string result;

		HMODULE hModule = NULL; // default to system source
		LPSTR MessageBuffer;
		DWORD dwBufferLength;

		DWORD dwFormatFlags = FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_IGNORE_INSERTS | FORMAT_MESSAGE_FROM_SYSTEM;

		//
		// If dwLastError is in the network range, 
		//  load the message source.
		//

		if (dwLastError >= NERR_BASE && dwLastError <= MAX_NERR) {
			hModule = LoadLibraryEx(TEXT("netmsg.dll"), NULL, LOAD_LIBRARY_AS_DATAFILE);
			if (hModule != NULL)
				dwFormatFlags |= FORMAT_MESSAGE_FROM_HMODULE;
		}

		//
		// Call FormatMessage() to allow for message 
		// text to be acquired from the system 
		// or from the supplied module handle.
		//
		if (dwBufferLength = FormatMessageA(
			dwFormatFlags,
			hModule, // module to get message from (NULL == system)
			dwLastError,
			MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), // default language
			(LPSTR)&MessageBuffer,
			0,
			NULL
			))
		{
			result = std::string(MessageBuffer);
			LocalFree(MessageBuffer);
		}

		//
		// If we loaded a message source, unload it.
		//
		if (hModule != NULL)
			FreeLibrary(hModule);

		return result;
	}
};

class Counter32 : public AbstractCounter
{
public:
	Counter32(PPERF_COUNTERSET_INSTANCE instance, ULONG id) :
		AbstractCounter(instance, id, &_counter)
	{
	}

	~Counter32()
	{
	}

	std::uint32_t& getCounter()
	{
		return _counter;
	}

private:
	std::uint32_t _counter = 0;
};

class Gauge32 : public AbstractCounter
{
public:
	Gauge32(PPERF_COUNTERSET_INSTANCE instance, ULONG id) :
		AbstractCounter(instance, id, &_counter)
	{
	}

	std::uint32_t& getGauge32()
	{
		return _counter;
	}

	~Gauge32()
	{
	}
private:
	std::uint32_t _counter = 0;
};

class Gauge64 : public AbstractCounter
{
public:
	Gauge64(PPERF_COUNTERSET_INSTANCE instance, ULONG id) :
		AbstractCounter(instance, id, &_counter)
	{
	}

	std::uint64_t& getGauge64()
	{
		return _counter;
	}

	~Gauge64()
	{
	}
private:
	std::uint64_t _counter = 0;
};

class CounterSet
{
public:
	CounterSet(LPGUID instanceId, std::wstring& name)
	{
		_instance = PerfCreateInstance(Yardmaster, instanceId, (std::wstring(L"Yardmaster.") + name).c_str(), 0);
		if (_instance == nullptr) {
			CounterCleanup();

			std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
			std::string nameAsChar = converter.to_bytes(name);

			std::stringstream errorMessage;
			errorMessage << "Error creating an instance of the counterset '" << nameAsChar << "' " << GetLastError();
			throw std::exception(errorMessage.str().c_str());
		}
	}

	~CounterSet()
	{
		PerfDeleteInstance(Yardmaster, _instance);
	}

protected:
	PPERF_COUNTERSET_INSTANCE _instance;
};

class CounterProvider
{
public:
	CounterProvider()
	{
		ULONG status = CounterInitialize(nullptr, nullptr, nullptr, nullptr);
		if (status != ERROR_SUCCESS) {
			std::stringstream errorMessage;
			errorMessage << "Error initializing performance counters:" << status;
			throw std::exception(errorMessage.str().c_str());
		}

	}

	virtual ~CounterProvider()
	{
		CounterCleanup();
	}
};

class PerformanceCounterSet : public CounterSet
{
public:
	PerformanceCounterSet(LPGUID instanceId, std::wstring& name) :
		CounterSet(instanceId, name)
	{
		_counter1 = std::unique_ptr<Counter32>(new Counter32(_instance, 1));
	}

	std::uint32_t& getCounter1()
	{
		return _counter1->getCounter();
	}

	~PerformanceCounterSet()
	{
	}

private:
	std::unique_ptr<Counter32> _counter1;
};

class YardmasterCounterProvider : public CounterProvider
{
public:
	YardmasterCounterProvider()
	{
		_performance = std::unique_ptr<PerformanceCounterSet>(new PerformanceCounterSet(&PerformanceGuid, std::wstring(L"Performance")));
	}

	PerformanceCounterSet& getPerformanceSet()
	{
		if (_performance) {
			return *_performance;
		}
		else
			throw std::exception("Access to uninitialized counterset 'Performance'");
	}
	~YardmasterCounterProvider()
	{
	}
private:
	std::unique_ptr<PerformanceCounterSet> _performance;
};

#endif // !__COUNTERSIMPL_HPP_
