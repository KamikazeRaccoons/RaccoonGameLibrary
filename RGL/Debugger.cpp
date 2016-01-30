#include "Debugger.h"
#include "Game.h"

namespace rgl
{
	Debugger* Debugger::m_pInstance = 0;

	Debugger* Debugger::get()
	{
		if (m_pInstance == 0)
			m_pInstance = new Debugger();

		return m_pInstance;
	}

	void Debugger::init(std::string title)
	{
		if (!Game::get()->isDebugEnabled() || m_initialized)
			return;

		AllocConsole();
		SetConsoleTitle((std::wstring(title.begin(), title.end()) + L" Debugger").c_str());
		m_hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

		m_initialized = true;
	}

	void Debugger::log(std::string message, Debugger::LogType logType)
	{
		if (!Game::get()->isDebugEnabled() || !m_initialized)
			return;

		if (m_lastLogType != logType)
		{
			m_lastLogType = logType;

			WORD textColor;

			switch (logType)
			{
			case Debugger::LogType::MESSAGE:
				textColor = FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE; // White
				break;
			case Debugger::LogType::WARNING:
				textColor = FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY; // Yellow
				break;
			case Debugger::LogType::FATAL_ERROR:
				textColor = FOREGROUND_RED | FOREGROUND_INTENSITY; // Red
				break;
			}

			SetConsoleTextAttribute(m_hConsole, textColor);
		}

		std::wstring wMessage = std::wstring(message.begin(), message.end()) + L"\n";
		WriteConsole(m_hConsole, wMessage.c_str(), wMessage.length(), NULL, NULL);
	}
}