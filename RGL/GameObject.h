#pragma once

#include "stdafx.h"
#include "ObjectParams.h"
#include "Vector2.h"

#ifdef RGL_EXPORTS
#define RGL_GAMEACTOR_API __declspec(dllexport)
#else
#define RGL_GAMEACTOR_API __declspec(dllimport)
#endif

namespace rgl
{
	class GameObject
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

		RGL_GAMEACTOR_API GameObject();
		virtual ~GameObject() { }

		virtual RGL_GAMEACTOR_API void load(const std::shared_ptr<ObjectParams> pObjectParams);

		virtual RGL_GAMEACTOR_API void update();
		virtual RGL_GAMEACTOR_API void draw();
		virtual void clean() { }

		RGL_GAMEACTOR_API Vector2& getPosition();
		RGL_GAMEACTOR_API int getWidth();
		RGL_GAMEACTOR_API int getHeight();

	};
}