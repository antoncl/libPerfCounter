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
#include <vector>
#include <string>
#include <boost/filesystem.hpp>

#include "Utilities.hpp"


namespace lib_perf_counter {

namespace fs = boost::filesystem;

// These function will only work and can only be compiled under windows
// the following code is *very* windows specific
#ifdef _WIN32

std::string findInEnvironmentPath(const std::string& fileName)
{
	// get the path and name of the console application (cmd.exe)
	wchar_t rawPathSetting[32767];								// raw wchar buffer
	GetEnvironmentVariable(L"Path", rawPathSetting, 32767);		// get environment variable into the raw buffer
	std::wstring pathSetting(rawPathSetting);					// convert wchar buffer to a wchar string

	// split semicolon separated wchar string into elements
	std::vector<std::wstring> pathSettingElements;
	boost::split(pathSettingElements, pathSetting, boost::is_any_of(L";"), boost::token_compress_on);

	// examine each potential path
	for (std::wstring pathElement : pathSettingElements) {
		if (pathElement.length() > 0) {
			fs::path candidate = fs::path(pathElement) / fileName;

			if (fs::exists(candidate))
				return candidate.string();
		}
	}

	// return an empty string
	return std::string();
}

std::string findCtrppExe()
{
	std::string exePath = findInEnvironmentPath(CTRPP_EXE);
	if (exePath.size() > 0)
		return exePath;

	// try the windows sdk environment variable
	wchar_t sdkDir[1024] = { '\0' };
	DWORD result = GetEnvironmentVariable(L"WindowsSdkDir", sdkDir, 512);
	if (result <= 1024) {
		fs::path winSdkPath = sdkDir / fs::path(L"bin\\x86\\" CTRPP_EXE);
		if (fs::exists(winSdkPath))
			return winSdkPath.string();
	}

	// attempt some known locations
	std::vector<std::string> knownLocations {
		R"(C:\Program Files\Microsoft SDKs\Windows\v7.0\Bin\" CTRPP_EXE ")",
		R"(C:\Program Files\Microsoft SDKs\Windows\v7.0\Bin\x64\" CTRPP_EXE ")",
		R"(C:\Program Files\Microsoft SDKs\Windows\v7.1\Bin\" CTRPP_EXE ")",
		R"(C:\Program Files\Microsoft SDKs\Windows\v7.1\Bin\x64\" CTRPP_EXE ")",
		R"(C:\Program Files (x86)\Microsoft SDKs\Windows\v7.0A\bin\" CTRPP_EXE ")",
		R"(C:\Program Files (x86)\Microsoft SDKs\Windows\v7.0A\bin\x64\" CTRPP_EXE ")",
		R"(C:\Program Files (x86)\Microsoft SDKs\Windows\v7.1A\Bin\" CTRPP_EXE ")",
		R"(C:\Program Files (x86)\Microsoft SDKs\Windows\v7.1A\Bin\x64\" CTRPP_EXE ")",
		R"(C:\Program Files (x86)\Windows Kits\10\bin\arm64\" CTRPP_EXE ")",
		R"(C:\Program Files (x86)\Windows Kits\10\bin\x64\" CTRPP_EXE ")",
		R"(C:\Program Files (x86)\Windows Kits\10\bin\x86\" CTRPP_EXE ")",
		R"(C:\Program Files (x86)\Windows Kits\8.0\bin\x64\" CTRPP_EXE ")",
		R"(C:\Program Files (x86)\Windows Kits\8.0\bin\x86\" CTRPP_EXE ")",
		R"(C:\Program Files (x86)\Windows Kits\8.1\bin\x64\" CTRPP_EXE ")",
		R"(C:\Program Files (x86)\Windows Kits\8.1\bin\x86\" CTRPP_EXE ")"
	};

	for (const std::string p : knownLocations) {
		fs::path fsPath(p);
		if (fs::exists(fsPath))
			return p;
	}

	return std::string();
}

bool containsSpaces(const std::string& s)
{
	return s.find(' ') != std::string::npos;
}

uint32_t shellExecute(const std::string& program, const std::string& parameters, const std::string& consoleOut, const std::string& errorOut)
{
	// get the path and name of the console application (cmd.exe)
	wchar_t comSpec[512];
	GetEnvironmentVariable(L"ComSpec", comSpec, 512);	// typically C:\Windows\System32\cmd.exe",

	std::wstring programAsWString;
	programAsWString.assign(program.begin(), program.end());

	std::wstring parametersAsWString;
	parametersAsWString.assign(parameters.begin(), parameters.end());

	std::wstring consoleOutAsWString;
	consoleOutAsWString.assign(consoleOut.begin(), consoleOut.end());

	std::wstring errorOutAsWString;
	errorOutAsWString.assign(errorOut.begin(), errorOut.end());

	std::wstring shellCmd(L"/C ");
	// executable
	if (containsSpaces(program)) {
		shellCmd = shellCmd.append(L"\"");
		shellCmd = shellCmd.append(L"\"");
	}

	shellCmd = shellCmd.append(programAsWString);

	if (containsSpaces(program)) {
		shellCmd = shellCmd.append(L"\"");
		if(!containsSpaces(parameters))
			shellCmd = shellCmd.append(L"\"");
	}

	// parameters
	shellCmd = shellCmd.append(L" ");
	shellCmd = shellCmd.append(parametersAsWString);

	if (containsSpaces(parameters))
		shellCmd = shellCmd.append(L"\"");

	// redirect std out
	if (consoleOut.size() > 0) {
		shellCmd = shellCmd.append(L" >");
		if (containsSpaces(consoleOut)) {
			shellCmd = shellCmd.append(L"\"" + consoleOutAsWString + L"\"");
		} else 
			shellCmd = shellCmd.append(consoleOutAsWString);
	}

	// redirect std err
	if (errorOut.size() > 0) {
		shellCmd = shellCmd.append(L" 2>");
		if (containsSpaces(errorOut)) {
			shellCmd = shellCmd.append(L"\"" + errorOutAsWString + L"\"");
		}
		else
			shellCmd = shellCmd.append(errorOutAsWString);
	}

	STARTUPINFO si;
	PROCESS_INFORMATION pi;

	ZeroMemory(&si, sizeof(si));
	si.cb = sizeof(si);
	ZeroMemory(&pi, sizeof(pi));

	if (CreateProcess(comSpec,
		(LPWSTR)shellCmd.c_str(),
		NULL,
		NULL,
		FALSE,
		0,
		NULL,
		NULL,
		&si, &pi))
	{
		::WaitForSingleObject(pi.hProcess, INFINITE);

		DWORD exitCode;
		GetExitCodeProcess(pi.hProcess, &exitCode);

		CloseHandle(pi.hProcess);
		CloseHandle(pi.hThread);

		if (exitCode) {
			return exitCode;
		}
	}
	else {
		return GetLastError();
	}

	return 0;
}

#endif

}