#pragma once

#include <rglGameState.h>

class OtherState :
	public rglGameState
{
private:

	static std::string m_stateID;

public:
	OtherState() { }
	~OtherState() { }

	virtual void update();
	virtual void render();

	virtual bool onEnter();
	virtual bool onExit();

	virtual std::string getStateID() const;
};

