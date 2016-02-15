#include "FileIO.h"
#include "Debugger.h"

namespace rgl
{
	std::string FileIO::readFile(std::string filename)
	{
		std::string result;
		std::ifstream file(filename);

		if (file.is_open())
		{
			while (!file.eof())
			{
				if (!result.empty())
					result += '\n';

				std::string currentLine;
				std::getline(file, currentLine);
				result += currentLine;
			}
		}
		else
		{
			rgl::Debugger::get()->log("Could not open script.", rgl::Debugger::WARNING);
		}

		return result;
	}

	void FileIO::writeFile(std::string filename, std::string value)
	{
		std::ofstream file(filename);
		file << value;
		file.close();
	}

	bool FileIO::fileExists(std::string filename)
	{
		std::ifstream file(filename);
		return file.good();
	}
}