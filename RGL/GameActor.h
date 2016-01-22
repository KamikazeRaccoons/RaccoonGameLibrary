#pragma once

#include "stdafx.h"
#include "ObjectParams.h"
#include "GameObject.h"
#include "Vector2.h"

namespace rgl
{
	class GameActor : public GameObject
	{
	protected:

		Vector2 m_position;
		Vector2 m_velocity;
		Vector2 m_acceleration;

		int m_width;
		int m_height;

		int m_currentRow;
		int m_currentFrame;

		int m_numFrames;

		std::string m_textureID;

	public:

		RGL_API GameActor(std::shared_ptr<Level> pParentLevel, int x, int y, int width, int height, std::string textureID, int numFrames);
		virtual ~GameActor() { }

		virtual void onCreate() { }
		virtual void onDestroy() { }

		virtual RGL_API void update();
		virtual RGL_API void draw();

		RGL_API Vector2& getPosition();
		RGL_API int getWidth();
		RGL_API int getHeight();

	};
}