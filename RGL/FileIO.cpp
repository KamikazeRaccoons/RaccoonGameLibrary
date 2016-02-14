#include "FileIO.h"
#include "Debugger.h"

namespace rgl
{
	std::string FileIO::readFile(std::string)
	{
		std::string result;
		std::ifstream file("assets/scripts/update.py");

		if (file.is_open())
		{
			while (!file.eof())
			{
				std::string currentLine;
				std::getline(file, currentLine);
				result += currentLine + "\n";
			}
		}
		else
		{
			rgl::Debugger::get()->log("Could not open script.", rgl::Debugger::WARNING);
		}

		return result;
	}
}