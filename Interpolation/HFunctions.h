#pragma once



#include <chrono>
#include <ctime>
#include <iomanip>
#include <stdlib.h>



#define MiB 1024*1024



#ifdef _WIN32

#include <mutex>
#include <windows.h>
#include <DbgHelp.h>
#include <Shlwapi.h>
#include <excpt.h>
#include <tchar.h>

#pragma comment (lib, "dbghelp.lib")
#pragma comment (lib, "shlwapi.lib")


#if defined( FoundSDK )
#define EXISTSDK 1
#endif


#endif






std::chrono::time_point<std::chrono::high_resolution_clock> start;
std::chrono::time_point<std::chrono::high_resolution_clock> end;

void StartTimer()
{
	
	start = std::chrono::high_resolution_clock::now();
}

std::chrono::duration<double>&  StopTimer()
{
	
	end = std::chrono::high_resolution_clock::now();
	std::chrono::duration<double> elapsed_seconds = end - start;

	return elapsed_seconds;
}



bool IsDefault(const char c)
{
	return c == 'd';
}


std::string GetStr(std::string message, size_t val)
{
	std::stringstream ss;
	ss << message;
	ss << val;
	return ss.str();
}

inline bool IsNegativeSubstraction(UInt64 N, UInt64 M)
{
	try
	{
		UInt64 nm = (N - M);
		return nm > N;
	}
	catch (...)
	{
		return true;
	}
}

inline bool IsOverflowAddition(UInt64 N, UInt64 M)
{
	try
	{
		UInt64 nm = (N + M);
		return nm < N;
	}
	catch (...)
	{
		return true;
	}
}

inline bool FileExist(const std::string& name) {
	struct stat buffer;
	return (stat(name.c_str(), &buffer) == 0);
}

void SetLocalWorkSize(bool IsLower, size_t argument)
{
	if (IsLower)
		argument = 16;
	else
		argument = 64;
}

void SetLocalWorkSize(bool IsLower, std::vector<size_t>& argument)
{
	const size_t size = argument.size();
	if (size > 1)
	{
		if (IsLower)
		{

			for (size_t index = 0; index < size; index++)
				argument[index] = 16;
		}
		else
		{
			for (size_t index = 0; index < size; index++)
				argument[index] = 64;
		}
	}
	else
	{
		if (IsLower)
			argument[0] = 16;
		else
			argument[0] = 64;
	}
}


template< class Type, size_t size>
void SetLocalWorkSize(bool IsLower, Type(&arguments)[size])
{



	if (IsLower)
	{

		for (size_t index = 0; index < size; index++)
			arguments[index] = 16;
	}
	else
	{
		for (size_t index = 0; index < size; index++)
			arguments[index] = 64;
	}


}