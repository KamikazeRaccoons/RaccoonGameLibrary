#pragma once

#include <rglGUIState.h>
#include <rglGameObject.h>
#include <rglDebugger.h>

class InitState : public rglGUIState
{
private:

	static string m_stateID;

	virtual void setCallbacks(const vector<Callback>& callbacks);

	static void onOtherStateClick();

public:

	InitState() { }
	~InitState() { }

	virtual void update();
	virtual void render();

	virtual void onEnter();
	virtual void onExit();

	virtual string getStateID() const;

};

