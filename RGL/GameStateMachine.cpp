#include "GameStateMachine.h"

namespace rgl
{
	void GameStateMachine::pollTransitions()
	{
		for (auto transition : m_queuedTransitions)
		{
			switch (transition.first)
			{
			case PUSH:
				m_gameStates.push_back(transition.second);
				m_gameStates.back()->onEnter();
				break;
			case CHANGE:
				if (!m_gameStates.empty())
				{
					m_gameStates.back()->onExit();
					m_gameStates.pop_back();
				}

				m_gameStates.push_back(transition.second);
				m_gameStates.back()->onEnter();
				break;
			case POP:
				if (!m_gameStates.empty())
				{
					m_gameStates.back()->onExit();
					m_gameStates.pop_back();
				}
				break;
			}
		}

		m_queuedTransitions.clear();
	}

	void GameStateMachine::pushState(std::shared_ptr<GameState> pGameState)
	{
		m_queuedTransitions.push_back(std::make_pair(PUSH, pGameState));
	}

	void GameStateMachine::changeState(std::shared_ptr<GameState> pGameState)
	{
		m_queuedTransitions.push_back(std::make_pair(CHANGE, pGameState));
	}

	void GameStateMachine::popState()
	{
		m_queuedTransitions.push_back(std::make_pair<TransitionType, std::shared_ptr<GameState>>(POP, 0));
	}

	void GameStateMachine::update()
	{
		pollTransitions();

		if (!m_gameStates.empty())
			m_gameStates.back()->update();
	}

	void GameStateMachine::render()
	{
		if (!m_gameStates.empty())
			m_gameStates.back()->render();
	}

	void GameStateMachine::clean()
	{
		while (!m_gameStates.empty())
		{
			popState();
			pollTransitions();
		}
	}
}