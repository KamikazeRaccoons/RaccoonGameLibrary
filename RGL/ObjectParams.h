#pragma once

#include "stdafx.h"

namespace rgl
{
	class ObjectParams
	{
	private:

		std::map<std::string, std::string> m_params;

	public:

		ObjectParams() { }
		~ObjectParams() { }

		RGL_API void setParam(std::string name, std::string value);
		RGL_API std::string getStringParam(std::string name);
		RGL_API int getIntParam(std::string name);
		RGL_API double getDoubleParam(std::string name);

	};
}