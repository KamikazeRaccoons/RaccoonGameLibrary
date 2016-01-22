#pragma once

#include "stdafx.h"

namespace rgl
{
	struct Debugger
	{
		enum LogType
		{
			MESSAGE = 0,
			WARNING = 1,
			FATAL_ERROR = 2
		};

		static RGL_API void log(std::string message, LogType logType = MESSAGE);

	};
}