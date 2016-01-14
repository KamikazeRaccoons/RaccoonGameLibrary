// .cpp : Defines the exported functions for the DLL application.
//

#include "stdafx.h"
#include "Game.h"
#include "InputHandler.h"
#include "TextureManager.h"
#include "Debugger.h"
#include "Vector2.h"
#include "GameState.h"
#include "GameStateMachine.h"
#include "Level.h"
#include "TileLayer.h"
#include "ObjectLayer.h"
#include "LevelParser.h"
#include "ObjectParams.h"
#include "ObjectFactory.h"
#include "GameObject.h"
#include "Button.h"

namespace rgl
{
	// Game

	Game* Game::m_pInstance = 0;

	Game* Game::get()
	{
		if (m_pInstance == 0)
			m_pInstance = new Game();

		return m_pInstance;
	}

	SDL_Window* Game::getWindow()
	{
		return m_pWindow;
	}

	SDL_Renderer* Game::getRenderer()
	{
		return m_pRenderer;
	}

	std::shared_ptr<GameStateMachine> Game::getGameStateMachine()
	{
		return m_pGameStateMachine;
	}

	void Game::setFrameRate(double frameRate)
	{
		m_deltaTime = 1.0 / frameRate;
	}

	double Game::getDeltaTime() const
	{
		return m_deltaTime;
	}

	int Game::getWidth() const
	{
		return m_width;
	}

	int Game::getHeight() const
	{
		return m_height;
	}

	bool Game::run(std::string title, int width, int height, std::shared_ptr<GameState> pInitState, bool fullscreen, double frameRate)
	{
		if (pInitState == 0 ||
			SDL_Init(SDL_INIT_EVERYTHING) < 0 ||
			(m_pWindow = SDL_CreateWindow(title.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height,
				fullscreen ? SDL_WINDOW_FULLSCREEN : SDL_WINDOW_SHOWN)) == 0 ||
			(m_pRenderer = SDL_CreateRenderer(m_pWindow, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC)) == 0)
			return false;

		m_width = width;
		m_height = height;
		m_deltaTime = 1.0 / frameRate;
		m_running = true;

		ObjectFactory::get()->registerType("Button", std::make_shared<ButtonCreator>());

		m_pGameStateMachine = std::make_shared<GameStateMachine>();
		m_pGameStateMachine->changeState(pInitState);

		double updateTime = SDL_GetTicks() * 0.001;
		double accumulatedTime = 0.0;

		while (m_running)
		{
			double currentTime = SDL_GetTicks() * 0.001;
			double frameTime = currentTime - updateTime;
			updateTime = currentTime;
			accumulatedTime += frameTime;

			while (accumulatedTime > m_deltaTime)
			{
				pollEvents();
				update();
				accumulatedTime -= m_deltaTime;
			}

			render();
		}

		clean();

		return true;
	}

	void Game::quit()
	{
		m_running = false;
	}

	void Game::pollEvents()
	{
		InputHandler::get()->update();
	}

	void Game::update()
	{
		m_pGameStateMachine->update();
	}

	void Game::render()
	{
		SDL_SetRenderDrawColor(m_pRenderer, 255, 255, 255, 255);
		SDL_RenderClear(m_pRenderer);

		m_pGameStateMachine->render();

		SDL_RenderPresent(m_pRenderer);
	}

	void Game::clean()
	{
		m_pGameStateMachine->clean();
		SDL_DestroyRenderer(m_pRenderer);
		SDL_DestroyWindow(m_pWindow);
		SDL_Quit();
	}

	// InputHandler

	InputHandler* InputHandler::m_pInstance = 0;

	InputHandler::InputHandler()
	{
		for (int i = 0; i < 3; i++)
			m_mouseButtonStates.push_back(false);

		m_pMousePosition = std::make_shared<Vector2>();
	}

	InputHandler* InputHandler::get()
	{
		if (m_pInstance == 0)
			m_pInstance = new InputHandler();

		return m_pInstance;
	}

	void InputHandler::onMouseButtonDown(SDL_Event& event)
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

	void InputHandler::onMouseButtonUp(SDL_Event& event)
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

	void InputHandler::onMouseMove(SDL_Event& event)
	{
		m_pMousePosition->setX(event.motion.x);
		m_pMousePosition->setY(event.motion.y);
	}

	bool InputHandler::getMouseButtonState(int buttonID)
	{
		return m_mouseButtonStates[buttonID];
	}

	std::shared_ptr<Vector2> InputHandler::getMousePosition()
	{
		return m_pMousePosition;
	}

	bool InputHandler::isKeyDown(SDL_Scancode key)
	{
		if (m_pKeystates == 0)
			return false;

		return m_pKeystates[key] == 1 ? true : false;
	}

	void InputHandler::update()
	{
		m_pKeystates = SDL_GetKeyboardState(0);

		SDL_Event event;

		while (SDL_PollEvent(&event))
		{
			switch (event.type)
			{
			case SDL_QUIT:
				Game::get()->quit();
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

	// TextueManager

	TextureManager* TextureManager::m_pInstance = 0;

	TextureManager* TextureManager::get()
	{
		if (m_pInstance == 0)
			m_pInstance = new TextureManager();

		return m_pInstance;
	}

	bool TextureManager::load(std::string fileName, std::string id)
	{
		SDL_Surface* pTempSurface = IMG_Load(fileName.c_str());

		if (pTempSurface == 0)
			return false;

		SDL_Texture* pTexture = SDL_CreateTextureFromSurface(Game::get()->getRenderer(), pTempSurface);

		SDL_FreeSurface(pTempSurface);

		if (pTexture != 0)
		{
			m_textures[id] = pTexture;
			return true;
		}

		return false;
	}

	void TextureManager::unload(std::string id)
	{
		SDL_DestroyTexture(m_textures[id]);
		m_textures.erase(id);
	}

	void TextureManager::draw(std::string id, int x, int y, int width, int height, SDL_RendererFlip renderFlip)
	{
		drawFrame(id, x, y, width, height, 0, 0, renderFlip);
	}

	void TextureManager::drawFrame(std::string id, int x, int y, int width, int height, int currentRow, int currentFrame,
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

		SDL_RenderCopyEx(Game::get()->getRenderer(), m_textures[id], &srcRect, &destRect, 0, 0, renderFlip);
	}

	void TextureManager::drawTile(std::string id, int margin, int spacing, int x, int y, int width, int height,
		int currentRow, int currentFrame)
	{
		SDL_Rect srcRect;
		SDL_Rect destRect;
		srcRect.x = margin + (spacing + width) * currentFrame;
		srcRect.y = margin + (spacing + height) * currentRow;
		srcRect.w = destRect.w = width;
		srcRect.h = destRect.h = height;
		destRect.x = x;
		destRect.y = y;

		SDL_RenderCopyEx(Game::get()->getRenderer(), m_textures[id], &srcRect, &destRect, 0, 0, SDL_FLIP_NONE);
	}

	// Debugger

	void Debugger::log(std::string message, Debugger::LogType logType)
	{
#ifdef _DEBUG
		HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
		CONSOLE_SCREEN_BUFFER_INFO consoleInfo;
		GetConsoleScreenBufferInfo(hConsole, &consoleInfo);
		WORD textColor;

		switch (logType)
		{
		case Debugger::LogType::MESSAGE:
			textColor = FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE; // White
			break;
		case Debugger::LogType::WARNING:
			textColor = FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY; // Yellow
			break;
		case Debugger::LogType::ERROR:
			textColor = FOREGROUND_RED | FOREGROUND_INTENSITY; // Red
			break;
		}

		SetConsoleTextAttribute(hConsole, textColor);
		std::cout << message << std::endl;
		SetConsoleTextAttribute(hConsole, consoleInfo.wAttributes);
#endif
	}

	// Vector2

	void Vector2::setX(double x)
	{
		m_x = x;
	}

	void Vector2::setY(double y)
	{
		m_y = y;
	}

	double Vector2::getX()
	{
		return m_x;
	}

	double Vector2::getY()
	{
		return m_y;
	}

	double Vector2::getLength()
	{
		return sqrt(m_x * m_x + m_y * m_y);
	}

	Vector2 Vector2::operator+(const Vector2& v2) const
	{
		return Vector2(m_x + v2.m_x, m_y + v2.m_y);
	}

	Vector2& Vector2::operator+=(const Vector2& v2)
	{
		m_x += v2.m_x;
		m_y += v2.m_y;

		return *this;
	}

	Vector2 Vector2::operator-(const Vector2& v2) const
	{
		return Vector2(m_x - v2.m_x, m_y - v2.m_y);
	}

	Vector2& Vector2::operator-=(const Vector2& v2)
	{
		m_x -= v2.m_x;
		m_y -= v2.m_y;

		return *this;
	}

	Vector2 Vector2::operator*(double scalar) const
	{
		return Vector2(m_x * scalar, m_y * scalar);
	}

	Vector2& Vector2::operator*=(double scalar)
	{
		m_x *= scalar;
		m_y *= scalar;

		return *this;
	}

	Vector2 Vector2::operator/(double scalar) const
	{
		return Vector2(m_x / scalar, m_y / scalar);
	}

	Vector2& Vector2::operator/=(double scalar)
	{
		m_x /= scalar;
		m_y /= scalar;

		return *this;
	}

	void Vector2::normalize()
	{
		double l = getLength();

		if (l > 0)
			*this *= 1.0 / l;
	}

	// GameStateMachine

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
			update();
		}
	}

	// Level

	void Level::update()
	{
		for (unsigned int i = 0; i < m_layers.size(); i++)
			m_layers[i]->update();
	}

	void Level::render()
	{
		for (unsigned int i = 0; i < m_layers.size(); i++)
			m_layers[i]->render();
	}

	void Level::clean()
	{
		for (unsigned int i = 0; i < m_layers.size(); i++)
			m_layers[i]->clean();

		for (unsigned int i = 0; i < m_textureIDs.size(); i++)
			rgl::TextureManager::get()->unload(m_textureIDs[i]);
	}

	void Level::addCallback(std::function<void()> callback)
	{
		m_callbacks.push_back(callback);
	}

	void Level::assignCallbacks()
	{
		for (unsigned int i = 0; i < m_layers.size(); i++)
		{
			std::shared_ptr<ObjectLayer> pObjectLayer = std::dynamic_pointer_cast<ObjectLayer>(m_layers[i]);

			if (!pObjectLayer)
				continue;

			for (unsigned int j = 0; j < pObjectLayer->getGameObjects().size(); j++)
			{
				std::shared_ptr<Button> pButton = std::dynamic_pointer_cast<Button>(pObjectLayer->getGameObjects()[j]);

				if (pButton && pButton->getCallbackID() < (int)m_callbacks.size())
					pButton->setCallback(m_callbacks[pButton->getCallbackID()]);
			}
		}
	}

	std::vector<Tileset>& Level::getTilesets()
	{
		
		return m_tilesets;
	}

	std::vector<std::shared_ptr<Layer>>& Level::getLayers()
	{
		return m_layers;
	}

	std::vector<std::string>& Level::getTextureIDs()
	{
		return m_textureIDs;
	}

	// TileLayer

	TileLayer::TileLayer(int tileSize, const std::vector<Tileset>& tilesets)
		: m_tileSize(tileSize), m_tilesets(tilesets), m_position(0, 0), m_velocity(0, 0)
	{
		m_numColumns = Game::get()->getWidth() / m_tileSize;
		m_numRows = Game::get()->getHeight() / m_tileSize;
	}

	void TileLayer::update()
	{
		m_velocity.setX(1);
	}

	void TileLayer::render()
	{
		int x, y, x2, y2;

		x = (int)(m_position.getX() / m_tileSize);
		y = (int)(m_position.getY() / m_tileSize);

		x2 = (int)m_position.getX() % m_tileSize;
		y2 = (int)m_position.getY() % m_tileSize;

		for (int i = 0; i < (y2 > 0 ? m_numRows + 1 : m_numRows); i++)
		{
			for (int j = 0; j < (x2 > 0 ? m_numColumns + 1 : m_numColumns); j++)
			{
				int id;

				if (i + y < (int)m_tileIDs.size() && j + x < (int)m_tileIDs[i + y].size())
					id = m_tileIDs[i + y][j + x];
				else
					id = 0;

				if (id == 0)
					continue;

				Tileset tileset = getTilesetByID(id);

				id--;

				TextureManager::get()->drawTile(tileset.name, tileset.margin, tileset.spacing,
					(j * m_tileSize) - x2, (i * m_tileSize) - y2, m_tileSize, m_tileSize,
					(id - (tileset.firstGridID - 1)) / max(tileset.numColumns, 1), (id - (tileset.firstGridID - 1)) % max(tileset.numColumns, 1));
			}
		}
	}

	void TileLayer::setTileIDs(const std::vector<std::vector<int>>& data)
	{
		m_tileIDs = data;
	}

	void TileLayer::setTileSize(int tileSize)
	{
		m_tileSize = tileSize;
	}

	Tileset TileLayer::getTilesetByID(int tileID)
	{
		for (unsigned int i = 0; i < m_tilesets.size(); i++)
		{
			if (i + 1 <= m_tilesets.size() - 1)
			{
				if (tileID >= m_tilesets[i].firstGridID && tileID < m_tilesets[i + 1].firstGridID)
					return m_tilesets[i];
			}
			else
			{
				return m_tilesets[i];
			}
		}

		Debugger::log("Could not find tileset with ID " + std::to_string(tileID) + ". Returning empty tileset.");

		return Tileset();
	}

	// ObjectLayer

	std::vector<std::shared_ptr<GameObject>>& ObjectLayer::getGameObjects()
	{
		return m_gameObjects;
	}

	void ObjectLayer::update()
	{
		for (unsigned int i = 0; i < m_gameObjects.size(); i++)
			m_gameObjects[i]->update();
	}

	void ObjectLayer::render()
	{
		for (unsigned int i = 0; i < m_gameObjects.size(); i++)
			m_gameObjects[i]->draw();
	}

	void ObjectLayer::clean()
	{
		for (unsigned int i = 0; i < m_gameObjects.size(); i++)
			m_gameObjects[i]->clean();

		m_gameObjects.clear();
	}

	// LevelParser

	void LevelParser::parseTilesets(tinyxml2::XMLElement* pTilesetRoot, std::vector<Tileset>& tilesets, std::vector<std::string>& textureIDs, std::string path)
	{
		std::string textureID = pTilesetRoot->Attribute("name");
		std::string textureSource = path + pTilesetRoot->FirstChildElement()->Attribute("source");

		if (TextureManager::get()->load(textureSource, textureID))
		{
			textureIDs.push_back(textureID);
		}
		else
		{
			Debugger::log("Could not load texture \"" + textureSource + "\".", Debugger::ERROR);
			return;
		}

		Tileset tileset;
		tileset.width = pTilesetRoot->FirstChildElement()->IntAttribute("width");
		tileset.height = pTilesetRoot->FirstChildElement()->IntAttribute("height");
		tileset.firstGridID = pTilesetRoot->IntAttribute("firstgid");
		tileset.tileWidth = pTilesetRoot->IntAttribute("tilewidth");
		tileset.tileHeight = pTilesetRoot->IntAttribute("tileheight");
		tileset.spacing = pTilesetRoot->IntAttribute("spacing");
		tileset.margin = pTilesetRoot->IntAttribute("margin");
		tileset.name = pTilesetRoot->Attribute("name");
		tileset.numColumns = tileset.width / (tileset.tileWidth + tileset.spacing);

		tilesets.push_back(tileset);
	}

	void LevelParser::parseTileLayer(tinyxml2::XMLElement* pTileElement, std::vector<std::shared_ptr<Layer>>& layers,
		const std::vector<Tileset>& tilesets)
	{
		std::shared_ptr<TileLayer> pTileLayer = std::make_shared<TileLayer>(m_tileSize, tilesets);

		std::vector<std::vector<int>> data;

		std::string decodedIDs;
		tinyxml2::XMLElement* pDataElement = 0;

		for (tinyxml2::XMLElement* e = pTileElement->FirstChildElement(); e != 0; e = e->NextSiblingElement())
		{
			if (e->Value() == std::string("data"))
				pDataElement = e;
		}

		if (!pDataElement)
		{
			Debugger::log("Could not find \"data\" element.", Debugger::ERROR);
			return;
		}

		std::string t = pDataElement->FirstChild()->ToText()->Value();
		t.erase(remove_if(t.begin(), t.end(), isspace), t.end());
		decodedIDs = base64_decode(t);

		uLongf numGids = m_width * m_height * sizeof(int);
		std::vector<unsigned int> gids(numGids);
		uncompress((Bytef*)&gids[0], &numGids, (const Bytef*)decodedIDs.c_str(), decodedIDs.size());

		std::vector<int> layerRow(m_width);

		for (int i = 0; i < m_height; i++)
			data.push_back(layerRow);

		for (int rows = 0; rows < m_height; rows++)
			for (int columns = 0; columns < m_width; columns++)
				data[rows][columns] = gids[rows * m_width + columns];

		pTileLayer->setTileIDs(data);

		layers.push_back(pTileLayer);
	}

	void LevelParser::parseTextures(tinyxml2::XMLElement* pPropertiesRoot, std::vector<std::string>& textureIDs, std::string path)
	{
		for (tinyxml2::XMLElement* e = pPropertiesRoot->FirstChildElement(); e != 0; e = e->NextSiblingElement())
		{
			if (e->Value() == std::string("property"))
			{
				std::string textureID = e->Attribute("name");
				std::string textureSource = path + e->Attribute("value");
				
				if (TextureManager::get()->load(textureSource, textureID))
					textureIDs.push_back(textureID);
				else
					Debugger::log("Could not load texture \"" + textureSource + "\".", Debugger::ERROR);
			}
		}
	}

	void LevelParser::parseObjectLayers(tinyxml2::XMLElement* pObjectGroupRoot, std::vector<std::shared_ptr<Layer>>& layers)
	{
		std::shared_ptr<ObjectLayer> pObjectLayer = std::make_shared<ObjectLayer>();

		for (tinyxml2::XMLElement* e = pObjectGroupRoot->FirstChildElement(); e != 0; e = e->NextSiblingElement())
		{
			if (e->Value() == std::string("object"))
			{
				std::shared_ptr<GameObject> pGameObject = ObjectFactory::get()->create(e->Attribute("type"));

				if (!pGameObject)
					continue;

				std::shared_ptr<ObjectParams> pParams = std::make_shared<ObjectParams>();

				for (const tinyxml2::XMLAttribute* a = e->FirstAttribute(); a != 0; a = a->Next())
					pParams->setParam(a->Name(), a->Value());

				for (tinyxml2::XMLElement* properties = e->FirstChildElement(); properties != 0; properties = properties->NextSiblingElement())
				{
					if (properties->Value() == std::string("properties"))
					{
						for (tinyxml2::XMLElement* property = properties->FirstChildElement(); property != 0; property = property->NextSiblingElement())
						{
							if (property->Value() == std::string("property"))
								pParams->setParam(property->Attribute("name"), property->Attribute("value"));
						}
					}
				}

				pGameObject->load(pParams);
				pObjectLayer->getGameObjects().push_back(pGameObject);
			}
		}

		layers.push_back(pObjectLayer);
	}

	std::shared_ptr<Level> LevelParser::parseLevel(std::string path, std::string file)
	{
		tinyxml2::XMLDocument xmlDoc;
		xmlDoc.LoadFile((path + file).c_str());

		std::shared_ptr<Level> pLevel = std::make_shared<Level>();

		tinyxml2::XMLElement* pRoot = xmlDoc.RootElement();

		m_tileSize = pRoot->IntAttribute("tilewidth");
		m_width = pRoot->IntAttribute("width");
		m_height = pRoot->IntAttribute("height");

		for (tinyxml2::XMLElement* e = pRoot->FirstChildElement(); e != 0; e = e->NextSiblingElement())
		{
			if (e->Value() == std::string("properties"))
				parseTextures(e, pLevel->getTextureIDs(), path);
			else if (e->Value() == std::string("tileset"))
				parseTilesets(e, pLevel->getTilesets(), pLevel->getTextureIDs(), path);
			else if (e->Value() == std::string("layer"))
				parseTileLayer(e, pLevel->getLayers(), pLevel->getTilesets());
			else if (e->Value() == std::string("objectgroup"))
				parseObjectLayers(e, pLevel->getLayers());
		}

		return pLevel;
	}

	// ObjectParams

	void ObjectParams::setParam(std::string name, std::string value)
	{
		m_params[name] = value;
	}

	std::string ObjectParams::getStringParam(std::string name)
	{
		return m_params[name];
	}

	int ObjectParams::getIntParam(std::string name)
	{
		return std::atoi(m_params[name].c_str());
	}

	double ObjectParams::getDoubleParam(std::string name)
	{
		return std::atof(m_params[name].c_str());
	}

	// ObjectFactory

	ObjectFactory* ObjectFactory::m_pInstance = 0;

	ObjectFactory* ObjectFactory::get()
	{
		if (m_pInstance == 0)
			m_pInstance = new ObjectFactory();

		return m_pInstance;
	}

	bool ObjectFactory::registerType(std::string typeID, std::shared_ptr<ObjectCreator> pCreator)
	{
		std::map<std::string, std::shared_ptr<ObjectCreator>>::iterator it = m_creators.find(typeID);

		if (it != m_creators.end())
			return false;

		m_creators[typeID] = pCreator;

		return true;
	}

	std::shared_ptr<GameObject> ObjectFactory::create(std::string typeID)
	{
		std::map<std::string, std::shared_ptr<ObjectCreator>>::iterator it = m_creators.find(typeID);

		if (it == m_creators.end())
		{
			Debugger::log("Type \"" + typeID + "\" has not been registered.", Debugger::ERROR);
			return 0;
		}

		std::shared_ptr<ObjectCreator> pCreator = it->second;
		return pCreator->createObject();
	}

	// GameObject

	GameObject::GameObject()
	{
	}

	void GameObject::load(const std::shared_ptr<ObjectParams> pObjectParams)
	{
		m_position = Vector2(pObjectParams->getIntParam("x"), pObjectParams->getIntParam("y"));

		m_velocity = Vector2(0, 0);
		m_acceleration = Vector2(0, 0);

		m_width = pObjectParams->getIntParam("width");
		m_height = pObjectParams->getIntParam("height");

		m_textureID = pObjectParams->getStringParam("textureID");

		m_currentRow = 0;
		m_currentFrame = 0;

		m_numFrames = pObjectParams->getIntParam("numFrames");
	}

	void GameObject::update()
	{
		m_velocity += m_acceleration;
		m_position += m_velocity;
	}

	void GameObject::draw()
	{
		TextureManager::get()->drawFrame(m_textureID, (int)m_position.getX(), (int)m_position.getY(),
			m_width, m_height, m_currentRow, m_currentFrame);
	}

	Vector2& GameObject::getPosition()
	{
		return m_position;
	}

	int GameObject::getWidth()
	{
		return m_width;
	}

	int GameObject::getHeight()
	{
		return m_height;
	}

	// Button

	void Button::load(const std::shared_ptr<ObjectParams> pObjectParams)
	{
		GameObject::load(pObjectParams);
		m_callbackID = pObjectParams->getIntParam("callbackID");
		m_currentFrame = MOUSE_AWAY;
	}

	void Button::update()
	{
		std::shared_ptr<Vector2> pMousePos = InputHandler::get()->getMousePosition();

		if (pMousePos->getX() < (m_position.getX() + m_width) &&
			pMousePos->getX() > m_position.getX() &&
			pMousePos->getY() < (m_position.getY() + m_height) &&
			pMousePos->getY() > m_position.getY())
		{
			if (InputHandler::get()->getMouseButtonState(InputHandler::LEFT))
			{
				if (!m_pressed)
				{
					m_currentFrame = MOUSE_PRESSED;

					if (m_callback)
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

	void Button::draw()
	{
		GameObject::draw();
	}

	void Button::clean()
	{
		GameObject::clean();
	}

	void Button::setCallback(std::function<void()> callback)
	{
		m_callback = callback;
	}

	int Button::getCallbackID()
	{
		return m_callbackID;
	}
}