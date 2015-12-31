#pragma once

#include "rglGameActor.h"
#include "rglObjectFactory.h"

#ifdef RGL_EXPORTS
#define RGL_BUTTON_API __declspec(dllexport)
#else
#define RGL_BUTTON_API __declspec(dllimport)
#endif

class rglButton : public rglGameActor
{
private:

	enum ButtonState
	{
		MOUSE_AWAY = 0,
		MOUSE_HOVERING = 1,
		MOUSE_PRESSED = 2
	};

	bool m_pressed;

	void (*m_callback)();
	int m_callbackID;
	
public:

	RGL_BUTTON_API rglButton();

	virtual RGL_BUTTON_API void load(const shared_ptr<rglObjectParams> pObjectParams);

	virtual RGL_BUTTON_API void update();
	virtual RGL_BUTTON_API void draw();
	virtual RGL_BUTTON_API void clean();

	RGL_BUTTON_API void setCallback(void (*callback)());
	RGL_BUTTON_API int getCallbackID();
};

class rglButtonCreator : public rglObjectCreator
{
	shared_ptr<rglGameObject> createObject() const
	{
		return make_shared<rglButton>();
	}
};