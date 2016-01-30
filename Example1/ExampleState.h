#pragma once

#include <RGL.h>

class ExampleState : public rgl::GameState
{
private:

	std::shared_ptr<rgl::Level> m_pLevel;

	bool m_pressed;

	int m_crateID;

	void spawnCrate();

	static void quitState();

public:

	ExampleState() : m_crateID(3) { }
	~ExampleState() { }

	virtual void update();
	virtual void render();

	virtual void onEnter();
	virtual void onExit();

	virtual std::string getStateID() const;

};

