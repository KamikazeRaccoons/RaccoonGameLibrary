#pragma once

#include <map>
#include <string>

namespace rgl
{
	class ObjectParams
	{
	private:

		std::map<std::string, std::string> m_params;

	public:

		ObjectParams() { }
		~ObjectParams() { }

		void setParam(std::string name, std::string value);
		std::string getStringParam(std::string name);
		int getIntParam(std::string name);
		double getDoubleParam(std::string name);

	};
}