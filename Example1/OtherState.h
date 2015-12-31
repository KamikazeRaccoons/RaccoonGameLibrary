#pragma once

#include <rglGUIState.h>
#include <rglGameObject.h>

class OtherState : public rglGUIState
{
private:

	static std::string m_stateID;

	virtual void setCallbacks(const vector<Callback>& callbacks);

	static void onInitStateClick();

public:
	OtherState() { }
	~OtherState() { }

	virtual void update();
	virtual void render();

	virtual void onEnter();
	virtual void onExit();

	virtual std::string getStateID() const;
};

