#pragma once

#include <rglGUIState.h>
#include <rglGameObject.h>
#include <rglDebugger.h>
#include <rglLevel.h>

class ExampleState : public rglGUIState
{
private:

	static string m_stateID;

	shared_ptr<rglLevel> m_pLevel;

public:

	ExampleState() { }
	~ExampleState() { }

	virtual void update();
	virtual void render();

	virtual void onEnter();
	virtual void onExit();

	virtual string getStateID() const;

};

