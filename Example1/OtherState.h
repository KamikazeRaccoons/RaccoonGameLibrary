#pragma once

#include <rglGameState.h>
#include <rglGameObject.h>

class OtherState :
	public rglGameState
{
private:

	static std::string m_stateID;

	std::vector<shared_ptr<rglGameObject>> m_gameObjects;

	static void onInitStateClick();

public:
	OtherState() { }
	~OtherState() { }

	virtual void update();
	virtual void render();

	virtual bool onEnter();
	virtual bool onExit();

	virtual std::string getStateID() const;
};

