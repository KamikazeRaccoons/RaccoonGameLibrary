#include "InitState.h"
#include <rglGame.h>
#include <rglStateParser.h>
#include <rglObjectFactory.h>
#include <rglInputHandler.h>
#include <rglTextureManager.h>
#include <rglDebugger.h>
#include <rglButton.h>
#include "OtherState.h"

std::string InitState::m_stateID = "InitState";

void InitState::update()
{
	for (unsigned int i = 0; i < m_gameObjects.size(); i++)
		m_gameObjects[i]->update();
}

void InitState::render()
{
	SDL_SetRenderDrawColor(rglGame::getInstance()->getRenderer(), 255, 0, 0, 255);
	SDL_RenderClear(rglGame::getInstance()->getRenderer());

	for (unsigned int i = 0; i < m_gameObjects.size(); i++)
		m_gameObjects[i]->draw();
}

void InitState::onEnter()
{
	rglDebugger::log("InitState Entering...");

	SDL_SetWindowTitle(rglGame::getInstance()->getWindow(), "InitState");

	rglStateParser stateParser;
	stateParser.parseState("assets/states/InitState.state", m_stateID, m_gameObjects, m_textureIDs);

	m_callbacks.push_back(onOtherStateClick);

	setCallbacks(m_callbacks);
}

void InitState::onExit()
{
	rglDebugger::log("InitState Exiting...");

	for (unsigned int i = 0; i < m_gameObjects.size(); i++)
		m_gameObjects[i]->clean();

	m_gameObjects.clear();

	for (unsigned int i = 0; i < m_textureIDs.size(); i++)
		rglTextureManager::getInstance()->unload(m_textureIDs[i]);
}

void InitState::setCallbacks(const vector<Callback>& callbacks)
{
	for (unsigned int i = 0; i < m_gameObjects.size(); i++)
	{
		shared_ptr<rglButton> pButton = dynamic_pointer_cast<rglButton>(m_gameObjects[i]);

		if (pButton)
			pButton->setCallback(m_callbacks[pButton->getCallbackID()]);
	}
}

std::string InitState::getStateID() const
{
	return m_stateID;
}

void InitState::onOtherStateClick()
{
	rglGame::getInstance()->getGameStateMachine()->changeState(make_shared<OtherState>());
}