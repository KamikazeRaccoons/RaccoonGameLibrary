#pragma once

#include "stdafx.h"

#ifdef RGL_EXPORTS
#define RGL_OBJECTPARAMS_API __declspec(dllexport)
#else
#define RGL_OBJECTPARAMS_API __declspec(dllimport)
#endif

class rglObjectParams
{
private:

	int m_x;
	int m_y;

	int m_width;
	int m_height;

	std::string m_textureID;

public:

	RGL_OBJECTPARAMS_API rglObjectParams(int x, int y, int width, int height, std::string textureID);

	RGL_OBJECTPARAMS_API int getX() const;
	RGL_OBJECTPARAMS_API int getY() const;

	RGL_OBJECTPARAMS_API int getWidth() const;
	RGL_OBJECTPARAMS_API int getHeight() const;

	RGL_OBJECTPARAMS_API std::string getTextureID() const;
};