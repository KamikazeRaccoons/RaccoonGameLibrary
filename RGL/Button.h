#pragma once

#include "GameActor.h"
#include "ObjectFactory.h"

#ifdef RGL_EXPORTS
#define RGL_BUTTON_API __declspec(dllexport)
#else
#define RGL_BUTTON_API __declspec(dllimport)
#endif

namespace rgl
{
	class Button : public GameActor
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

		RGL_BUTTON_API Button(std::shared_ptr<Level> pParentLevel, int x, int y, int width, int height, std::string textureID, int callbackID);

		virtual RGL_BUTTON_API void onCreate();
		virtual RGL_BUTTON_API void onDestroy();

		virtual RGL_BUTTON_API void update();
		virtual RGL_BUTTON_API void draw();
		virtual RGL_BUTTON_API void clean();

		RGL_BUTTON_API void setCallback(std::function<void()> callback);
		RGL_BUTTON_API int getCallbackID();
	};

	class ButtonCreator : public ObjectCreator
	{
		std::shared_ptr<GameObject> createObject(std::shared_ptr<Level> pParentLevel, const std::shared_ptr<ObjectParams> pObjectParams) const
		{
			return std::make_shared<Button>(pParentLevel, pObjectParams->getIntParam("x"), pObjectParams->getIntParam("y"),
				pObjectParams->getIntParam("width"), pObjectParams->getIntParam("height"), pObjectParams->getStringParam("textureID"),
				pObjectParams->getIntParam("callbackID"));
		}
	};
}