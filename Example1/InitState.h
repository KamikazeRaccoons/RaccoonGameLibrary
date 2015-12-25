#pragma once

#include <rglGameState.h>
#include <rglGameObject.h>
#include <rglDebugger.h>

class InitState :
	public rglGameState
{
private:

	static std::string m_stateID;

	std::vector<shared_ptr<rglGameObject>> m_gameObjects;

	static void onOtherStateClick();

public:

	InitState() { }
	~InitState() { }

	virtual void update();
	virtual void render();

	virtual bool onEnter();
	virtual bool onExit();

	virtual std::string getStateID() const;

};

