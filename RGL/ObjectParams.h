#pragma once

#include "stdafx.h"

#ifdef RGL_EXPORTS
#define RGL_OBJECTPARAMS_API __declspec(dllexport)
#else
#define RGL_OBJECTPARAMS_API __declspec(dllimport)
#endif

namespace rgl
{
	class ObjectParams
	{
	private:

		std::map<std::string, std::string> m_params;

	public:

		ObjectParams() { }
		~ObjectParams() { }

		RGL_OBJECTPARAMS_API void setParam(std::string name, std::string value);
		RGL_OBJECTPARAMS_API std::string getStringParam(std::string name);
		RGL_OBJECTPARAMS_API int getIntParam(std::string name);
		RGL_OBJECTPARAMS_API double getDoubleParam(std::string name);

	};
}