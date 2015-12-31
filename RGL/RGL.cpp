// RGL.cpp : Defines the exported functions for the DLL application.
//

#include "stdafx.h"
#include "rglGame.h"
#include "rglInputHandler.h"
#include "rglTextureManager.h"
#include "rglDebugger.h"
#include "rglVector2.h"
#include "rglGameState.h"
#include "rglGameStateMachine.h"
#include "rglStateParser.h"
#include "rglObjectParams.h"
#include "rglObjectFactory.h"
#include "rglGameObject.h"
#include "rglGameActor.h"
#include "rglButton.h"

// rglGame

rglGame* rglGame::m_pInstance = 0;

rglGame* rglGame::getInstance()
{
	if (m_pInstance == 0)
		m_pInstance = new rglGame();

	return m_pInstance;
}

SDL_Window* rglGame::getWindow()
{
	return m_pWindow;
}

SDL_Renderer* rglGame::getRenderer()
{
	return m_pRenderer;
}

shared_ptr<rglGameStateMachine> rglGame::getGameStateMachine()
{
	return m_pGameStateMachine;
}

void rglGame::setFrameRate(double frameRate)
{
	m_frameRate = frameRate;
}

double rglGame::getFrameRate()
{
	return m_frameRate;
}

bool rglGame::run(string title, int width, int height, shared_ptr<rglGameState> pInitState, bool fullscreen, double frameRate)
{
	if (pInitState == 0 ||
		SDL_Init(SDL_INIT_EVERYTHING) < 0 ||
		(m_pWindow = SDL_CreateWindow(title.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height,
		fullscreen ? SDL_WINDOW_FULLSCREEN : SDL_WINDOW_SHOWN)) == 0 ||
		(m_pRenderer = (SDL_CreateRenderer(m_pWindow, -1, 0))) == 0)
		return false;

	m_frameRate = 1.0 / frameRate;

	m_running = true;

	rglObjectFactory::getInstance()->registerType("rglButton", make_shared<rglButtonCreator>());

	m_pGameStateMachine = make_shared<rglGameStateMachine>();
	m_pGameStateMachine->changeState(pInitState);

	double updateTime = SDL_GetTicks() * 0.001;
	double accumulatedTime = 0.0;

	while (m_running)
	{
		double currentTime = SDL_GetTicks() * 0.001;
		double frameTime = currentTime - updateTime;
		updateTime = currentTime;
		accumulatedTime += frameTime;

		while (accumulatedTime > m_frameRate)
		{
			pollEvents();
			update();
			accumulatedTime -= m_frameRate;
		}
		
		render();
	}

	clean();

	return true;
}

void rglGame::quit()
{
	m_running = false;
}

void rglGame::pollEvents()
{
	rglInputHandler::getInstance()->update();
}

void rglGame::update()
{
	m_pGameStateMachine->update();
}

void rglGame::render()
{
	SDL_SetRenderDrawColor(m_pRenderer, 255, 255, 255, 255);
	SDL_RenderClear(m_pRenderer);

	m_pGameStateMachine->render();

	SDL_RenderPresent(m_pRenderer);
}

void rglGame::clean()
{
	SDL_DestroyRenderer(m_pRenderer);
	SDL_DestroyWindow(m_pWindow);
	SDL_Quit();
}

// rglInputHandler

rglInputHandler* rglInputHandler::m_pInstance = 0;

rglInputHandler::rglInputHandler()
{
	for (int i = 0; i < 3; i++)
		m_mouseButtonStates.push_back(false);

	m_pMousePosition = make_shared<rglVector2>();
}

rglInputHandler* rglInputHandler::getInstance()
{
	if (m_pInstance == 0)
		m_pInstance = new rglInputHandler();

	return m_pInstance;
}

void rglInputHandler::onMouseButtonDown(SDL_Event& event)
{
	switch (event.button.button)
	{
	case SDL_BUTTON_LEFT:
		m_mouseButtonStates[LEFT] = true;
		break;
	case SDL_BUTTON_MIDDLE:
		m_mouseButtonStates[MIDDLE] = true;
		break;
	case SDL_BUTTON_RIGHT:
		m_mouseButtonStates[RIGHT] = true;
		break;
	}
}

void rglInputHandler::onMouseButtonUp(SDL_Event& event)
{
	switch (event.button.button)
	{
	case SDL_BUTTON_LEFT:
		m_mouseButtonStates[LEFT] = false;
		break;
	case SDL_BUTTON_MIDDLE:
		m_mouseButtonStates[MIDDLE] = false;
		break;
	case SDL_BUTTON_RIGHT:
		m_mouseButtonStates[RIGHT] = false;
		break;
	}
}

void rglInputHandler::onMouseMove(SDL_Event& event)
{
	m_pMousePosition->setX(event.motion.x);
	m_pMousePosition->setY(event.motion.y);
}

bool rglInputHandler::getMouseButtonState(int buttonID)
{
	return m_mouseButtonStates[buttonID];
}

shared_ptr<rglVector2> rglInputHandler::getMousePosition()
{
	return m_pMousePosition;
}

bool rglInputHandler::isKeyDown(SDL_Scancode key)
{
	if (m_pKeystates == 0)
		return false;

	return m_pKeystates[key] == 1 ? true : false;
}

void rglInputHandler::update()
{
	m_pKeystates = SDL_GetKeyboardState(0);
	
	SDL_Event event;

	while (SDL_PollEvent(&event))
	{
		switch (event.type)
		{
		case SDL_QUIT:
			rglGame::getInstance()->quit();
			break;
		case SDL_MOUSEBUTTONDOWN:
			onMouseButtonDown(event);
			break;
		case SDL_MOUSEBUTTONUP:
			onMouseButtonUp(event);
			break;
		case SDL_MOUSEMOTION:
			onMouseMove(event);
			break;
		}
	}
}

// rglTextueManager

rglTextureManager* rglTextureManager::m_pInstance = 0;

rglTextureManager* rglTextureManager::getInstance()
{
	if (m_pInstance == 0)
		m_pInstance = new rglTextureManager();

	return m_pInstance;
}

bool rglTextureManager::load(string fileName, string id)
{
	SDL_Surface* pTempSurface = IMG_Load(fileName.c_str());

	if (pTempSurface == 0)
		return false;

	SDL_Texture* pTexture = SDL_CreateTextureFromSurface(rglGame::getInstance()->getRenderer(), pTempSurface);

	SDL_FreeSurface(pTempSurface);

	if (pTexture != 0)
	{
		m_textures[id] = pTexture;
		return true;
	}

	return false;
}

void rglTextureManager::unload(string id)
{
	SDL_DestroyTexture(m_textures[id]);
	m_textures.erase(id);
}

void rglTextureManager::draw(string id, int x, int y, int width, int height, SDL_RendererFlip renderFlip)
{
	drawFrame(id, x, y, width, height, 0, 0, renderFlip);
}

void rglTextureManager::drawFrame(string id, int x, int y, int width, int height, int currentRow, int currentFrame,
								  SDL_RendererFlip renderFlip)
{
	SDL_Rect srcRect;
	SDL_Rect destRect;

	srcRect.x = width * currentFrame;
	srcRect.y = height * currentRow;
	srcRect.w = destRect.w = width;
	srcRect.h = destRect.h = height;
	destRect.x = x;
	destRect.y = y;

	SDL_RenderCopyEx(rglGame::getInstance()->getRenderer(), m_textures[id], &srcRect, &destRect, 0, 0, renderFlip);
}

// rglDebugger

void rglDebugger::log(string message, rglDebugger::LogType logType)
{
#ifdef _DEBUG
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_SCREEN_BUFFER_INFO consoleInfo;
	GetConsoleScreenBufferInfo(hConsole, &consoleInfo);
	WORD textColor;

	switch (logType)
	{
	case rglDebugger::LogType::MESSAGE:
		textColor = FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE; // White
		break;
	case rglDebugger::LogType::WARNING:
		textColor = FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY; // Yellow
		break;
	case rglDebugger::LogType::ERROR:
		textColor = FOREGROUND_RED | FOREGROUND_INTENSITY; // Red
		break;
	}

	SetConsoleTextAttribute(hConsole, textColor);
	cout << message << endl;
	SetConsoleTextAttribute(hConsole, consoleInfo.wAttributes);
#endif
}

// rglVector2

void rglVector2::setX(double x)
{
	m_x = x;
}

void rglVector2::setY(double y)
{
	m_y = y;
}

double rglVector2::getX()
{
	return m_x;
}

double rglVector2::getY()
{
	return m_y;
}

double rglVector2::getLength()
{
	return sqrt(m_x * m_x + m_y * m_y);
}

rglVector2 rglVector2::operator+(const rglVector2& v2) const
{
	return rglVector2(m_x + v2.m_x, m_y + v2.m_y);
}

rglVector2& rglVector2::operator+=(const rglVector2& v2)
{
	m_x += v2.m_x;
	m_y += v2.m_y;

	return *this;
}

rglVector2 rglVector2::operator-(const rglVector2& v2) const
{
	return rglVector2(m_x - v2.m_x, m_y - v2.m_y);
}

rglVector2& rglVector2::operator-=(const rglVector2& v2)
{
	m_x -= v2.m_x;
	m_y -= v2.m_y;

	return *this;
}

rglVector2 rglVector2::operator*(double scalar) const
{
	return rglVector2(m_x * scalar, m_y * scalar);
}

rglVector2& rglVector2::operator*=(double scalar)
{
	m_x *= scalar;
	m_y *= scalar;

	return *this;
}

rglVector2 rglVector2::operator/(double scalar) const
{
	return rglVector2(m_x / scalar, m_y / scalar);
}

rglVector2& rglVector2::operator/=(double scalar)
{
	m_x /= scalar;
	m_y /= scalar;

	return *this;
}

void rglVector2::normalize()
{
	double l = getLength();

	if (l > 0)
		*this *= 1.0 / l;
}

// rglGameStateMachine

void rglGameStateMachine::pushState(shared_ptr<rglGameState> pGameState)
{
	m_queuedTransitions.push_back(make_pair(PUSH, pGameState));
}

void rglGameStateMachine::changeState(shared_ptr<rglGameState> pGameState)
{
	m_queuedTransitions.push_back(make_pair(CHANGE, pGameState));
}

void rglGameStateMachine::popState()
{
	m_queuedTransitions.push_back(make_pair<TransitionType, shared_ptr<rglGameState>>(POP, 0));
}

void rglGameStateMachine::update()
{
	if (!m_gameStates.empty())
		m_gameStates.back()->update();

	for (unsigned int i = 0; i < m_queuedTransitions.size(); i++)
	{
		switch (m_queuedTransitions[i].first)
		{
		case PUSH:
			m_gameStates.push_back(m_queuedTransitions[i].second);
			m_gameStates.back()->onEnter();
			break;
		case CHANGE:
			if (!m_gameStates.empty())
			{
				if (m_gameStates.back()->getStateID() == m_queuedTransitions[i].second->getStateID())
					break;

				m_gameStates.back()->onExit();
				m_gameStates.pop_back();
			}

			m_gameStates.push_back(m_queuedTransitions[i].second);
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

void rglGameStateMachine::render()
{
	if (!m_gameStates.empty())
		m_gameStates.back()->render();
}

// rglStateParser

void rglStateParser::parseTextures(XMLElement* pTextureRoot, vector<string>& textureIDs)
{
	for (XMLElement* e = pTextureRoot->FirstChildElement(); e != 0; e = e->NextSiblingElement())
	{
		if (e->Value() != string("Texture"))
		{
			rglDebugger::log("Unknown element: \"" + string(e->Value()) + "\".", rglDebugger::WARNING);
			continue;
		}

		string filename = e->Attribute("filename");
		string id = e->Attribute("ID");
		textureIDs.push_back(id);

		rglTextureManager::getInstance()->load(filename, id);
	}
}

void rglStateParser::parseObjects(XMLElement* pStateRoot, vector<shared_ptr<rglGameObject>>& objects)
{
	for (XMLElement* e = pStateRoot->FirstChildElement(); e != 0; e = e->NextSiblingElement())
	{
		if (e->Value() != string("Object"))
		{
			rglDebugger::log("Unknown element: \"" + string(e->Value()) + "\".", rglDebugger::WARNING);
			continue;
		}

		int x = e->IntAttribute("x");
		int y = e->IntAttribute("y");
		int width = e->IntAttribute("width");
		int height = e->IntAttribute("height");
		int numFrames = e->IntAttribute("numFrames");
		int callbackID = e->IntAttribute("callbackID");
		int animSpeed = e->IntAttribute("animSpeed");
		string textureID = e->Attribute("textureID");

		shared_ptr<rglGameObject> pGameObject = rglObjectFactory::getInstance()->create(e->Attribute("type"));
		pGameObject->load(make_shared<rglObjectParams>(rglObjectParams(x, y, width, height, numFrames, textureID, callbackID, animSpeed)));
		objects.push_back(pGameObject);
	}
}

bool rglStateParser::parseState(const char* file, string stateID, vector<shared_ptr<rglGameObject>>& objects,
		vector<string>& textureIDs)
{
	XMLDocument xmlDoc;

	if (xmlDoc.LoadFile(file))
	{
		rglDebugger::log(xmlDoc.ErrorName(), rglDebugger::ERROR);
		return false;
	}

	XMLElement* pRoot = xmlDoc.RootElement();

	if (pRoot->Value() != stateID)
	{
		rglDebugger::log("State ID mismatch: \"" + string(pRoot->Value()) + "\" != \"" + stateID + "\".", rglDebugger::ERROR);
		return false;
	}

	for (XMLElement* e = pRoot->FirstChildElement(); e != 0; e = e->NextSiblingElement())
	{
		if (e->Value() == string("Textures"))
			parseTextures(e, textureIDs);
		else if (e->Value() == string("Objects"))
			parseObjects(e, objects);
		else
			rglDebugger::log("Unknown element: \"" + string(e->Value()) + "\".", rglDebugger::WARNING);
	}

	return true;
}

// rglObjectParams

rglObjectParams::rglObjectParams(int x, int y, int width, int height, int numFrames, string textureID,
								 int callbackID, int animSpeed)
	: m_x(x), m_y(y), m_width(width), m_height(height), m_numFrames(numFrames), m_textureID(textureID),
	m_callbackID(callbackID), m_animSpeed(animSpeed)
{
}

int rglObjectParams::getX() const
{
	return m_x;
}

int rglObjectParams::getY() const
{
	return m_y;
}

int rglObjectParams::getWidth() const
{
	return m_width;
}

int rglObjectParams::getHeight() const
{
	return m_height;
}

int rglObjectParams::getNumFrames() const
{
	return m_numFrames;
}

int rglObjectParams::getAnimSpeed() const
{
	return m_animSpeed;
}

int rglObjectParams::getCallbackID() const
{
	return m_callbackID;
}

string rglObjectParams::getTextureID() const
{
	return m_textureID;
}

// rglObjectFactory

rglObjectFactory* rglObjectFactory::m_pInstance = 0;

rglObjectFactory* rglObjectFactory::getInstance()
{
	if (m_pInstance == 0)
		m_pInstance = new rglObjectFactory();

	return m_pInstance;
}

bool rglObjectFactory::registerType(string typeID, shared_ptr<rglObjectCreator> pCreator)
{
	map<string, shared_ptr<rglObjectCreator>>::iterator it = m_creators.find(typeID);

	if (it != m_creators.end())
		return false;

	m_creators[typeID] = pCreator;

	return true;
}

shared_ptr<rglGameObject> rglObjectFactory::create(string typeID)
{
	map<string, shared_ptr<rglObjectCreator>>::iterator it = m_creators.find(typeID);

	if (it == m_creators.end())
	{
		rglDebugger::log("Type \"" + typeID + "\" has not been registered.", rglDebugger::ERROR);
		return 0;
	}

	shared_ptr<rglObjectCreator> pCreator = it->second;
	return pCreator->createObject();
}

// rglGameActor

rglGameActor::rglGameActor()
	: rglGameObject()
{
}

void rglGameActor::load(const shared_ptr<rglObjectParams> pObjectParams)
{
	m_position = rglVector2(pObjectParams->getX(), pObjectParams->getY());

	m_velocity = rglVector2(0, 0);
	m_acceleration = rglVector2(0, 0);

	m_width = pObjectParams->getWidth();
	m_height = pObjectParams->getHeight();

	m_textureID = pObjectParams->getTextureID();

	m_currentRow = 0;
	m_currentFrame = 0;

	m_numFrames = pObjectParams->getNumFrames();
}

void rglGameActor::update()
{
	m_velocity += m_acceleration;
	m_position += m_velocity;
}

void rglGameActor::draw()
{
	rglTextureManager::getInstance()->drawFrame(m_textureID, (int)m_position.getX(), (int)m_position.getY(),
		m_width, m_height, m_currentRow, m_currentFrame);
}

rglVector2& rglGameActor::getPosition()
{
	return m_position;
}

int rglGameActor::getWidth()
{
	return m_width;
}

int rglGameActor::getHeight()
{
	return m_height;
}

// rglButton

rglButton::rglButton()
	: rglGameActor()
{
}

void rglButton::load(const shared_ptr<rglObjectParams> pObjectParams)
{
	rglGameActor::load(pObjectParams);
	m_callbackID = pObjectParams->getCallbackID();
	m_currentFrame = MOUSE_AWAY;
}

void rglButton::update()
{
	shared_ptr<rglVector2> pMousePos = rglInputHandler::getInstance()->getMousePosition();

	if (pMousePos->getX() < (m_position.getX() + m_width) &&
		pMousePos->getX() > m_position.getX() &&
		pMousePos->getY() < (m_position.getY() + m_height) &&
		pMousePos->getY() > m_position.getY())
	{
		if (rglInputHandler::getInstance()->getMouseButtonState(rglInputHandler::LEFT))
		{
			if (!m_pressed)
			{
				m_currentFrame = MOUSE_PRESSED;

				m_callback();
				
				m_pressed = true;
			}
		}
		else
		{
			m_pressed = false;
			m_currentFrame = MOUSE_HOVERING;
		}
	}
	else
	{
		m_currentFrame = MOUSE_AWAY;
	}
}

void rglButton::draw()
{
	rglGameActor::draw();
}

void rglButton::clean()
{
	rglGameActor::clean();
}

void rglButton::setCallback(void (*callback)())
{
	m_callback = callback;
}

int rglButton::getCallbackID()
{
	return m_callbackID;
}