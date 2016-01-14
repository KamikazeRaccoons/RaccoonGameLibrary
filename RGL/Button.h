#pragma once

#include "GameObject.h"
#include "ObjectFactory.h"

#ifdef RGL_EXPORTS
#define RGL_BUTTON_API __declspec(dllexport)
#else
#define RGL_BUTTON_API __declspec(dllimport)
#endif

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
		};

		bool m_pressed;

		std::function<void()> m_callback;
		int m_callbackID;

	public:

		Button() : GameObject() { }

		virtual RGL_BUTTON_API void load(const std::shared_ptr<ObjectParams> pObjectParams);

		virtual RGL_BUTTON_API void update();
		virtual RGL_BUTTON_API void draw();
		virtual RGL_BUTTON_API void clean();

		RGL_BUTTON_API void setCallback(std::function<void()> callback);
		RGL_BUTTON_API int getCallbackID();
	};

	class ButtonCreator : public ObjectCreator
	{
		std::shared_ptr<GameObject> createObject() const
		{
			return std::make_shared<Button>();
		}
	};
}