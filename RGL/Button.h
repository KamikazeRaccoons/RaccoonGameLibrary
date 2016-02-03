#pragma once

#include <functional>
#include <memory>
#include <string>

#include "GameObject.h"
#include "ObjectFactory.h"

namespace rgl
{
	class Button : public GameObject
	{
	private:

		enum ButtonState
		{
			MOUSE_AWAY = 0,
			MOUSE_HOVERING = 1,
			MOUSE_PRESSED = 2
		} m_buttonState;

		Vector2 m_position;

		int m_width;
		int m_height;

		std::string m_textureID;

		int m_callbackID;
		std::function<void()> m_callback;

	public:

		Button(int x, int y, int width, int height, std::string textureID, int callbackID, std::string name)
			: GameObject(name), m_buttonState(MOUSE_AWAY), m_position(x, y), m_width(width), m_height(height),
			m_textureID(textureID), m_callbackID(callbackID) { }

		virtual void onCreate();
		virtual void onDestroy() { }

		virtual void update();
		virtual void draw();

		virtual int getX();
		virtual int getY();

		virtual int getWidth();
		virtual int getHeight();
	};

	class ButtonCreator : public ObjectCreator
	{
		std::shared_ptr<GameObject> createObject(const std::shared_ptr<ObjectParams> pObjectParams, std::string name) const
		{
			return std::make_shared<Button>(pObjectParams->getIntParam("x"), pObjectParams->getIntParam("y"),
				pObjectParams->getIntParam("width"), pObjectParams->getIntParam("height"), pObjectParams->getStringParam("textureID"),
				pObjectParams->getIntParam("callbackID"), name);
		}
	};
}