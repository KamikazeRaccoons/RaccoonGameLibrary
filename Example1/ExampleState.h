#pragma once

#include <RGL.h>

class ExampleState : public rgl::GameState
{
private:

	static std::string m_stateID;

	std::shared_ptr<rgl::Level> m_pLevel;

	bool m_pressed;

	static void quitState();
	static void playShoot();

public:

	ExampleState() { }
	~ExampleState() { }

	virtual void update();
	virtual void render();

	virtual void onEnter();
	virtual void onExit();

	virtual std::string getStateID() const;

};

