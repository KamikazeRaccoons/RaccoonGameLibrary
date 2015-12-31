#pragma once

#include "stdafx.h"
#include "rglGameObject.h"
#include "rglObjectParams.h"
#include "rglVector2.h"

#ifdef RGL_EXPORTS
#define RGL_GAMEACTOR_API __declspec(dllexport)
#else
#define RGL_GAMEACTOR_API __declspec(dllimport)
#endif

class rglGameActor : public rglGameObject
{
protected:

	rglVector2 m_position;
	rglVector2 m_velocity;
	rglVector2 m_acceleration;

	int m_width;
	int m_height;

	int m_currentRow;
	int m_currentFrame;

	int m_numFrames;

	std::string m_textureID;

public:

	RGL_GAMEACTOR_API rglGameActor();

	virtual RGL_GAMEACTOR_API void load(const shared_ptr<rglObjectParams> pObjectParams);

	virtual RGL_GAMEACTOR_API void update();
	virtual RGL_GAMEACTOR_API void draw();
	virtual void clean() { }

	RGL_GAMEACTOR_API rglVector2& getPosition();
	RGL_GAMEACTOR_API int getWidth();
	RGL_GAMEACTOR_API int getHeight();

};