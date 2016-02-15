#pragma once

#include <string>
#include <fstream>

namespace rgl
{
	class FileIO
	{
	public:

		static std::string readFile(std::string filename);
		static void writeFile(std::string filename, std::string value);
		static bool fileExists(std::string filename);

	};
}
