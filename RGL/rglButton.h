#pragma once

#include "rglGameActor.h"

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

	void (*m_onClick)();
	
public:

	RGL_BUTTON_API rglButton(const shared_ptr<rglObjectParams> pObjectParams, void (*onClick)());

	virtual RGL_BUTTON_API void update();
	virtual RGL_BUTTON_API void draw();
	virtual RGL_BUTTON_API void clean();
};