#pragma once

#include <rglGameState.h>
#include <rglGameObject.h>

class OtherState :
	public rglGameState
{
private:

	static std::string m_stateID;

	vector<shared_ptr<rglGameObject>> m_gameObjects;

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

