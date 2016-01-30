#include "ObjectParams.h"

namespace rgl
{
	void ObjectParams::setParam(std::string name, std::string value)
	{
		m_params[name] = value;
	}

	std::string ObjectParams::getStringParam(std::string name)
	{
		return m_params[name];
	}

	int ObjectParams::getIntParam(std::string name)
	{
		return std::atoi(m_params[name].c_str());
	}

	double ObjectParams::getDoubleParam(std::string name)
	{
		return std::atof(m_params[name].c_str());
	}
}