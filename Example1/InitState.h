#pragma once

#include <rglGameState.h>

class InitState :
	public rglGameState
{
private:

	static std::string m_stateID;

public:

	InitState() { }
	~InitState() { }

	virtual void update();
	virtual void render();

	virtual bool onEnter();
	virtual bool onExit();

	virtual std::string getStateID() const;

};

