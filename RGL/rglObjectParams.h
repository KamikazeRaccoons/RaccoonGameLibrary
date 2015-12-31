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

	int m_numFrames;
	int m_animSpeed;

	int m_callbackID;

	std::string m_textureID;

public:

	RGL_OBJECTPARAMS_API rglObjectParams(int x, int y, int width, int height, int m_numFrames, std::string textureID, int callbackID = 0, int animSpeed = 0);

	RGL_OBJECTPARAMS_API int getX() const;
	RGL_OBJECTPARAMS_API int getY() const;

	RGL_OBJECTPARAMS_API int getWidth() const;
	RGL_OBJECTPARAMS_API int getHeight() const;

	RGL_OBJECTPARAMS_API int getNumFrames() const;
	RGL_OBJECTPARAMS_API int getAnimSpeed() const;

	RGL_OBJECTPARAMS_API int getCallbackID() const;

	RGL_OBJECTPARAMS_API std::string getTextureID() const;
};