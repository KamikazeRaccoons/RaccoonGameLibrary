#pragma once

#include <RGL.h>

class ExampleState : public rgl::GUIState
{
private:

	static std::string m_stateID;

	std::shared_ptr<rgl::Level> m_pLevel;

public:

	ExampleState() { }
	~ExampleState() { }

	virtual void update();
	virtual void render();

	virtual void onEnter();
	virtual void onExit();

	virtual std::string getStateID() const;

};

