#pragma once

#include "stdafx.h"

#ifdef RGL_EXPORTS
#define RGL_DEBUGGER_API __declspec(dllexport)
#else
#define RGL_DEBUGGER_API __declspec(dllimport)
#endif

#undef ERROR

struct rglDebugger
{
	enum LogType
	{
		MESSAGE = 0,
		WARNING = 1,
		ERROR = 2
	};

	static RGL_DEBUGGER_API void log(std::string message, LogType logType = MESSAGE);

};