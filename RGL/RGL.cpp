// .cpp : Defines the exported functions for the DLL application.
//

#include "stdafx.h"
#include "Game.h"
#include "InputHandler.h"
#include "TextureManager.h"
#include "SoundManager.h"
#include "Debugger.h"
#include "DebugDraw.h"
#include "Vector2.h"
#include "GameStateMachine.h"
#include "Level.h"
#include "TileLayer.h"
#include "ObjectLayer.h"
#include "LevelParser.h"
#include "ObjectParams.h"
#include "ObjectFactory.h"
#include "GameObject.h"
#include "Button.h"
#include "PhysicsObject.h"

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

	bool Game::isDebugEnabled() const
	{
		return m_useDebugging;
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

	bool Game::run(std::string title, int width, int height, std::shared_ptr<GameState> pInitState, bool useDebugging, bool fullscreen, double frameRate)
	{
		if (m_running)
			return false;

		m_useDebugging = useDebugging;

		Debugger::get()->init(title);

		if (pInitState == 0 ||
			SDL_Init(SDL_INIT_EVERYTHING) < 0 ||
			(m_pWindow = SDL_CreateWindow(title.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height,
				fullscreen ? SDL_WINDOW_FULLSCREEN : SDL_WINDOW_SHOWN)) == 0 ||
			(m_pRenderer = SDL_CreateRenderer(m_pWindow, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC)) == 0)
			return false;

		SoundManager::get()->init();

		m_width = width;
		m_height = height;
		m_deltaTime = 1.0 / frameRate;
		m_running = true;

		ObjectFactory::get()->registerType("Button", std::make_shared<ButtonCreator>());
		ObjectFactory::get()->registerType("PhysicsObject", std::make_shared<PhysicsObjectCreator>());

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
		SoundManager::get()->clean();

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

	// SoundManager

	SoundManager* SoundManager::m_pInstance = 0;

	SoundManager* SoundManager::get()
	{
		if (m_pInstance == 0)
			m_pInstance = new SoundManager();

		return m_pInstance;
	}

	void SoundManager::init()
	{
		Mix_OpenAudio(22050, MIX_DEFAULT_FORMAT, 2, 1024);
	}

	bool SoundManager::load(std::string fileName, std::string id, SoundManager::SoundType soundType)
	{
		if (soundType == MUSIC)
		{
			Mix_Music* pMusic = Mix_LoadMUS(fileName.c_str());

			if (pMusic == 0)
			{
				Debugger::get()->log("Could not load music from file \"" + fileName + "\".\n" + Mix_GetError(), Debugger::WARNING);
				return false;
			}

			m_music[id] = pMusic;
		}
		else if (soundType == SFX)
		{
			Mix_Chunk* pChunk = Mix_LoadWAV(fileName.c_str());

			if (pChunk == 0)
			{
				Debugger::get()->log("Could not load SFX from file \"" + fileName + "\".\n" + Mix_GetError(), Debugger::WARNING);
				return false;
			}

			m_sfxs[id] = pChunk;
		}

		return true;
	}

	void SoundManager::clean()
	{
		for (auto music : m_music)
			Mix_FreeMusic(music.second);

		m_music.clear();

		for (auto sfx : m_sfxs)
			Mix_FreeChunk(sfx.second);

		m_sfxs.clear();

		Mix_CloseAudio();
	}

	void SoundManager::playMusic(std::string id, int loops)
	{
		Mix_PlayMusic(m_music[id], loops);
	}

	void SoundManager::playSound(std::string id, int loops)
	{
		Mix_PlayChannel(-1, m_sfxs[id], loops);
	}

	// Debugger

	Debugger* Debugger::m_pInstance = 0;

	Debugger* Debugger::get()
	{
		if (m_pInstance == 0)
			m_pInstance = new Debugger();

		return m_pInstance;
	}

	void Debugger::init(std::string title)
	{
		if (!Game::get()->isDebugEnabled() || m_initialized)
			return;

		AllocConsole();
		SetConsoleTitle((std::wstring(title.begin(), title.end()) + L" Debugger").c_str());
		m_hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

		m_initialized = true;
	}

	void Debugger::log(std::string message, Debugger::LogType logType)
	{
		if (!Game::get()->isDebugEnabled() || !m_initialized)
			return;

		if (m_lastLogType != logType)
		{
			m_lastLogType = logType;

			WORD textColor;

			switch (logType)
			{
			case Debugger::LogType::MESSAGE:
				textColor = FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE; // White
				break;
			case Debugger::LogType::WARNING:
				textColor = FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY; // Yellow
				break;
			case Debugger::LogType::FATAL_ERROR:
				textColor = FOREGROUND_RED | FOREGROUND_INTENSITY; // Red
				break;
			}

			SetConsoleTextAttribute(m_hConsole, textColor);
		}
		
		std::wstring wMessage = std::wstring(message.begin(), message.end()) + L"\n";
		WriteConsole(m_hConsole, wMessage.c_str(), wMessage.length(), NULL, NULL);
	}

	// DebugDraw

	void DebugDraw::DrawPolygon(const b2Vec2* vertices, int32 vertexCount, const b2Color& color)
	{
		SDL_Point* points = new SDL_Point[vertexCount];

		for (int i = 0; i < vertexCount; i++)
		{
			points[i].x = (int)(vertices[i].x * m_pLevel->getTileSize() - m_pLevel->getPosition().getX());
			points[i].y = (int)(vertices[i].y * m_pLevel->getTileSize() - m_pLevel->getPosition().getY());
		}

		SDL_SetRenderDrawColor(Game::get()->getRenderer(), (int)(color.r * 255.0f), (int)(color.g * 255.0f), (int)(color.b * 255.0f), (int)(color.a * 255.0f));
		SDL_RenderDrawLines(Game::get()->getRenderer(), points, vertexCount);
		SDL_RenderDrawLine(Game::get()->getRenderer(), points[vertexCount - 1].x, points[vertexCount - 1].y, points[0].x, points[0].y);
	}

	void DebugDraw::DrawSolidPolygon(const b2Vec2* vertices, int32 vertexCount, const b2Color& color)
	{
		DrawPolygon(vertices, vertexCount, color);
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
					if (m_gameStates.back()->getStateID() == transition.second->getStateID())
						break;

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

	// Level

	Level::Level(int tileSize, int width, int height) : m_tileSize(tileSize), m_width(width), m_height(height),
		m_position(0, 0), m_velocity(0, 0), m_world(b2Vec2(0.0f, 9.81f))
	{
		DebugDraw* debugDraw = new DebugDraw(this);
		debugDraw->AppendFlags(b2Draw::e_shapeBit);
		m_world.SetDebugDraw(debugDraw);
	}

	void Level::pollOperations()
	{
		for (auto operation : m_queuedOperations)
		{
			switch (std::get<0>(operation))
			{
			case ADD:
				{
					std::vector<std::shared_ptr<ObjectLayer>> objectLayers;

					for (auto layer : m_layers)
					{
						std::shared_ptr<ObjectLayer> pObjectLayer = std::dynamic_pointer_cast<ObjectLayer>(layer);

						if (pObjectLayer)
							objectLayers.push_back(pObjectLayer);
					}

					if (std::get<2>(operation) < (int)objectLayers.size())
					{
						if (std::get<2>(operation) < 0)
							objectLayers.back()->getGameObjects().push_back(std::get<1>(operation));
						else
							objectLayers[std::get<2>(operation)]->getGameObjects().push_back(std::get<1>(operation));

						std::get<1>(operation)->onCreate();
					}
				}
				break;
			case REMOVE:
				for (auto layer : m_layers)
				{
					std::shared_ptr<ObjectLayer> pObjectLayer = std::dynamic_pointer_cast<ObjectLayer>(layer);

					if (pObjectLayer)
					{
						std::vector<std::shared_ptr<GameObject>>& gameObjects = pObjectLayer->getGameObjects();
						std::get<1>(operation)->onDestroy();
						gameObjects.erase(std::remove(gameObjects.begin(), gameObjects.end(), std::get<1>(operation)), gameObjects.end());
					}
				}
				break;
			}
		}

		m_queuedOperations.clear();
	}

	void Level::update()
	{
		m_position += m_velocity;

		m_world.Step((float)Game::get()->getDeltaTime(), 6, 2);

		for (auto layer : m_layers)
			layer->update();

		pollOperations();
	}

	void Level::render()
	{
		for (auto layer : m_layers)
			layer->render();

		if (Game::get()->isDebugEnabled())
			m_world.DrawDebugData();
	}

	void Level::clean()
	{
		for (auto layer : m_layers)
			layer->clean();

		for (auto textureID : m_textureIDs)
			rgl::TextureManager::get()->unload(textureID);
	}

	int Level::getTileSize() const
	{
		return m_tileSize;
	}

	int Level::getWidth() const
	{
		return m_width;
	}

	int Level::getHeight() const
	{
		return m_height;
	}

	void Level::addCallback(std::function<void()> callback)
	{
		m_callbacks.push_back(callback);
	}

	std::function<void()> Level::getCallback(int callbackID)
	{
		if (callbackID >= (int)m_callbacks.size())
			return std::function<void()>();

		return m_callbacks[callbackID];
	}

	void Level::addLayer(std::shared_ptr<Layer> pLayer)
	{
		m_layers.push_back(pLayer);
	}

	void Level::addTexture(std::string file, std::string textureID)
	{
		if (TextureManager::get()->load(file, textureID))
			m_textureIDs.push_back(textureID);
		else
			Debugger::get()->log("Could not load texture \"" + file + "\".", Debugger::WARNING);
	}

	void Level::addObject(std::shared_ptr<GameObject> pObject, int objectLayer)
	{
		pObject->setParentLevel(shared_from_this());
		m_queuedOperations.push_back(std::make_tuple(ADD, pObject, objectLayer));
	}

	void Level::removeObject(std::shared_ptr<GameObject> pObject)
	{
		m_queuedOperations.push_back(std::make_tuple(REMOVE, pObject, 0));
	}

	Tileset Level::getTilesetByID(int tileID)
	{
		for (unsigned int i = 0; i < m_tilesets.size(); i++)
		{
			if (i + 1 <= m_tilesets.size() - 1)
			{
				if (tileID >=m_tilesets[i].firstGridID && tileID < m_tilesets[i + 1].firstGridID)
					return m_tilesets[i];
			}
			else
			{
				return m_tilesets[i];
			}
		}

		Debugger::get()->log("Could not find tileset with ID " + std::to_string(tileID) + ". Returning empty tileset.");

		return Tileset();
	}

	std::vector<Tileset>& Level::getTilesets()
	{
		return m_tilesets;
	}

	Vector2& Level::getPosition()
	{
		return m_position;
	}

	Vector2& Level::getVelocity()
	{
		return m_velocity;
	}

	b2World& Level::getWorld()
	{
		return m_world;
	}

	// TileLayer

	TileLayer::TileLayer(std::shared_ptr<Level> pLevel)
		: Layer(pLevel), m_tileSize(pLevel->getTileSize())
	{
		m_numColumns = (int)std::ceil((float)Game::get()->getWidth() / (float)m_tileSize) + 1;
		m_numRows = (int)std::ceil((float)Game::get()->getHeight() / (float)m_tileSize) + 1;
	}

	void TileLayer::render()
	{
		int x, y, x2, y2;

		x = (int)(m_pLevel->getPosition().getX() / m_tileSize);
		y = (int)(m_pLevel->getPosition().getY() / m_tileSize);

		x2 = (int)m_pLevel->getPosition().getX() % m_tileSize;
		y2 = (int)m_pLevel->getPosition().getY() % m_tileSize;

		for (int i = 0; i < m_numRows; i++)
		{
			for (int j = 0; j < m_numColumns; j++)
			{
				int id;

				if (i + y >= 0 && i + y < (int)m_tileIDs.size() && j + x >= 0 && j + x < (int)m_tileIDs[i + y].size())
					id = m_tileIDs[i + y][j + x];
				else
					id = 0;

				if (id == 0)
					continue;

				Tileset tileset = m_pLevel->getTilesetByID(id);

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

	void TileLayer::generateBodies()
	{
		for (unsigned int y = 0; y < m_tileIDs.size(); y++)
		{
			for (unsigned int x = 0; x < m_tileIDs[y].size(); x++)
			{
				if (m_tileIDs[y][x] == 0)
					continue;

				b2BodyDef bodyDef;
				bodyDef.type = b2_staticBody;
				bodyDef.position.Set((float)x + 0.5f, (float)y + 0.5f);

				b2PolygonShape shape;
				shape.SetAsBox(0.5f, 0.5f);

				b2Body* pBody = m_pLevel->getWorld().CreateBody(&bodyDef);
				pBody->CreateFixture(&shape, 0.0f);
				m_pBodies.push_back(pBody);
			}
		}
	}

	// ObjectLayer

	std::vector<std::shared_ptr<GameObject>>& ObjectLayer::getGameObjects()
	{
		return m_gameObjects;
	}

	void ObjectLayer::update()
	{
		for (auto object : m_gameObjects)
			object->update();
	}

	void ObjectLayer::render()
	{
		for (auto object : m_gameObjects)
			object->draw();
	}

	void ObjectLayer::clean()
	{
		for (auto object : m_gameObjects)
			object->onDestroy();

		m_gameObjects.clear();
	}

	// LevelParser

	void LevelParser::parseTilesets(tinyxml2::XMLElement* pTilesetRoot, std::shared_ptr<Level> pLevel, std::string path)
	{
		pLevel->addTexture(path + pTilesetRoot->FirstChildElement()->Attribute("source"), pTilesetRoot->Attribute("name"));

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

		pLevel->getTilesets().push_back(tileset);
	}

	void LevelParser::parseTileLayer(tinyxml2::XMLElement* pTileElement, std::shared_ptr<Level> pLevel)
	{
		std::shared_ptr<TileLayer> pTileLayer = std::make_shared<TileLayer>(pLevel);

		std::vector<std::vector<int>> data;

		bool isInteractive = false;

		std::string decodedIDs;
		tinyxml2::XMLElement* pDataElement = 0;

		for (tinyxml2::XMLElement* e = pTileElement->FirstChildElement(); e != 0; e = e->NextSiblingElement())
		{
			if (e->Value() == std::string("properties"))
			{
				for (tinyxml2::XMLElement* p = e->FirstChildElement(); p != 0; p = p->NextSiblingElement())
				{
					if (p->Name() == std::string("property"))
					{
						std::string nameAttribute = p->Attribute("name");

						if (nameAttribute == std::string("interactive"))
							isInteractive = p->BoolAttribute("value");
					}
				}
			}
			else if (e->Value() == std::string("data"))
			{
				pDataElement = e;
			}
		}

		if (!pDataElement)
		{
			Debugger::get()->log("Could not find \"data\" element.", Debugger::WARNING);
			return;
		}

		std::string t = pDataElement->FirstChild()->ToText()->Value();
		t.erase(remove_if(t.begin(), t.end(), isspace), t.end());
		decodedIDs = base64_decode(t);

		uLongf numGids = pLevel->getWidth() * pLevel->getHeight() * sizeof(int);
		std::vector<unsigned int> gids(numGids);
		uncompress((Bytef*)&gids[0], &numGids, (const Bytef*)decodedIDs.c_str(), decodedIDs.size());

		std::vector<int> layerRow(pLevel->getWidth());

		for (int i = 0; i < pLevel->getHeight(); i++)
			data.push_back(layerRow);

		for (int rows = 0; rows < pLevel->getHeight(); rows++)
			for (int columns = 0; columns < pLevel->getWidth(); columns++)
				data[rows][columns] = gids[rows * pLevel->getWidth() + columns];

		pTileLayer->setTileIDs(data);

		if (isInteractive)
			pTileLayer->generateBodies();

		pLevel->addLayer(pTileLayer);
	}

	void LevelParser::parseProperties(tinyxml2::XMLElement* pPropertiesRoot, std::shared_ptr<Level> pLevel, std::string path)
	{
		for (tinyxml2::XMLElement* e = pPropertiesRoot->FirstChildElement(); e != 0; e = e->NextSiblingElement())
		{
			if (e->Value() == std::string("property"))
			{
				std::string nameAttribute = e->Attribute("name");

				if (nameAttribute == "gravity")
					pLevel->getWorld().SetGravity(b2Vec2(0.0f, e->FloatAttribute("value")));
				else
					pLevel->addTexture(path + e->Attribute("value"), nameAttribute);
			}
		}
	}
	
	void LevelParser::parseObjectLayers(tinyxml2::XMLElement* pObjectGroupRoot, std::shared_ptr<Level> pLevel)
	{
		pLevel->addLayer(std::make_shared<ObjectLayer>(pLevel));

		for (tinyxml2::XMLElement* e = pObjectGroupRoot->FirstChildElement(); e != 0; e = e->NextSiblingElement())
		{
			if (e->Value() == std::string("object"))
			{
				const char* nameAttribute = e->Attribute("name");
				const char* typeAttribute = e->Attribute("type");

				if (typeAttribute == 0)
				{
					Debugger::get()->log("Undefined type for object \"" + (nameAttribute == 0 ? "(unnamed)" : std::string(nameAttribute)) + "\".", Debugger::WARNING);
					continue;
				}
				
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

				pLevel->addObject(ObjectFactory::get()->create(typeAttribute, pParams));
			}
		}
	}

	std::shared_ptr<Level> LevelParser::parseLevel(std::string path, std::string file)
	{
		tinyxml2::XMLDocument xmlDoc;
		xmlDoc.LoadFile((path + file).c_str());

		tinyxml2::XMLElement* pRoot = xmlDoc.RootElement();

		std::shared_ptr<Level> pLevel = std::make_shared<Level>(pRoot->IntAttribute("tilewidth"), pRoot->IntAttribute("width"), pRoot->IntAttribute("height"));

		for (tinyxml2::XMLElement* e = pRoot->FirstChildElement(); e != 0; e = e->NextSiblingElement())
		{
			if (e->Value() == std::string("properties"))
				parseProperties(e, pLevel, path);
			else if (e->Value() == std::string("tileset"))
				parseTilesets(e, pLevel, path);
			else if (e->Value() == std::string("layer"))
				parseTileLayer(e, pLevel);
			else if (e->Value() == std::string("objectgroup"))
				parseObjectLayers(e, pLevel);
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

	std::shared_ptr<GameObject> ObjectFactory::create(std::string typeID, const std::shared_ptr<ObjectParams> pObjectParams)
	{
		std::map<std::string, std::shared_ptr<ObjectCreator>>::iterator it = m_creators.find(typeID);

		if (it == m_creators.end())
		{
			Debugger::get()->log("Type \"" + typeID + "\" has not been registered.", Debugger::WARNING);
			return 0;
		}
		
		return it->second->createObject(pObjectParams);
	}

	// GameObject

	void GameObject::setParentLevel(std::shared_ptr<Level> pParentLevel)
	{
		if (!m_pLevel)
			m_pLevel = pParentLevel;
	}

	// Button

	void Button::onCreate()
	{
		m_callback = m_pLevel->getCallback(m_callbackID);
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
				m_buttonState = MOUSE_PRESSED;
			}
			else
			{
				if (m_buttonState == MOUSE_PRESSED)
					m_callback();

				m_buttonState = MOUSE_HOVERING;
			}
		}
		else
		{
			m_buttonState = MOUSE_AWAY;
		}
	}
	
	void Button::draw()
	{
		TextureManager::get()->drawFrame(m_textureID, (int)m_position.getX(), (int)m_position.getY(), m_width, m_height, 0, m_buttonState);
	}

	Vector2& Button::getPosition()
	{
		return m_position;
	}

	int Button::getWidth()
	{
		return m_width;
	}

	int Button::getHeight()
	{
		return m_height;
	}

	// PhysicsObject

	void PhysicsObject::onCreate()
	{
		m_bodyDef.type = b2_dynamicBody;
		m_bodyDef.position.Set((float)(m_x + m_width / 2) / (float)m_pLevel->getTileSize(), (float)(m_y + m_height / 2) / (float)m_pLevel->getTileSize());
		m_shape.SetAsBox((float)m_width / (float)m_pLevel->getTileSize() * 0.5f, (float)m_height / (float)m_pLevel->getTileSize() * 0.5f);
		m_pBody = m_pLevel->getWorld().CreateBody(&m_bodyDef);
		m_pBody->CreateFixture(&m_shape, 0.0f);
	}

	void PhysicsObject::onDestroy()
	{
		m_pLevel->getWorld().DestroyBody(m_pBody);
	}

	void PhysicsObject::update()
	{
		m_x = (int)(m_pBody->GetPosition().x * m_pLevel->getTileSize()) - m_width / 2;
		m_y = (int)(m_pBody->GetPosition().y * m_pLevel->getTileSize()) - m_height / 2;
	}

	void PhysicsObject::draw()
	{
		TextureManager::get()->draw(m_textureID, m_x - (int)m_pLevel->getPosition().getX(), m_y - (int)m_pLevel->getPosition().getY(), m_width, m_height);
	}
}