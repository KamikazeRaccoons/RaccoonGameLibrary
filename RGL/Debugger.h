#pragma once

#include <string>

#include <Windows.h>

namespace rgl
{
	class Debugger
	{
	private:

		static Debugger* m_pInstance;
		
		bool m_initialized;

		HANDLE m_hConsole;

		Debugger() : m_lastLogType(MESSAGE) { }

	public:

		enum LogType
		{
			MESSAGE = 0,
			WARNING = 1,
			FATAL_ERROR = 2
		} m_lastLogType;
		
		static Debugger* get();

		void init(std::string title);
		void log(std::string message, LogType logType = MESSAGE);

	};
}