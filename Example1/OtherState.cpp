#include "OtherState.h"
#include <rglGame.h>
#include <rglStateParser.h>
#include <rglInputHandler.h>
#include <rglTextureManager.h>
#include <rglButton.h>
#include <rglDebugger.h>
#include "InitState.h"

std::string OtherState::m_stateID = "OtherState";

void OtherState::update()
{
	for (unsigned int i = 0; i < m_gameObjects.size(); i++)
		m_gameObjects[i]->update();
}

void OtherState::render()
{
	SDL_SetRenderDrawColor(rglGame::getInstance()->getRenderer(), 0, 255, 0, 255);
	SDL_RenderClear(rglGame::getInstance()->getRenderer());

	for (unsigned int i = 0; i < m_gameObjects.size(); i++)
		m_gameObjects[i]->draw();
}

void OtherState::onEnter()
{
	rglDebugger::log("OtherState Entering...");

	SDL_SetWindowTitle(rglGame::getInstance()->getWindow(), "OtherState");

	rglStateParser stateParser;
	stateParser.parseState("assets/states/OtherState.state", m_stateID, m_gameObjects, m_textureIDs);

	m_callbacks.push_back(onInitStateClick);

	setCallbacks(m_callbacks);
}

void OtherState::onExit()
{
	rglDebugger::log("OtherState Exiting...");

	for (unsigned int i = 0; i < m_gameObjects.size(); i++)
		m_gameObjects[i]->clean();

	m_gameObjects.clear();

	for (unsigned int i = 0; i < m_textureIDs.size(); i++)
		rglTextureManager::getInstance()->unload(m_textureIDs[i]);
}

void OtherState::setCallbacks(const vector<Callback>& callbacks)
{
	for (unsigned int i = 0; i < m_gameObjects.size(); i++)
	{
		shared_ptr<rglButton> pButton = dynamic_pointer_cast<rglButton>(m_gameObjects[i]);

		if (pButton)
			pButton->setCallback(m_callbacks[pButton->getCallbackID()]);
	}
}

std::string OtherState::getStateID() const
{
	return m_stateID;
}

void OtherState::onInitStateClick()
{
	rglGame::getInstance()->getGameStateMachine()->changeState(make_shared<InitState>());
}