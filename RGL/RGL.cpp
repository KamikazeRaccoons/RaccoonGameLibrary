// RGL.cpp : Defines the exported functions for the DLL application.
//

#include "stdafx.h"
#include "rglGame.h"
#include "rglTextureManager.h"
#include "rglVector2.h"
#include "rglObjectParams.h"
#include "rglGameObject.h"
#include "rglGameActor.h"

// rglGame

rglGame* rglGame::m_pInstance = 0;

rglGame* rglGame::getInstance()
{
	if (m_pInstance == 0)
		m_pInstance = new rglGame();

	return m_pInstance;
}

SDL_Renderer* rglGame::getRenderer()
{
	return m_pRenderer;
}

void rglGame::setFrameRate(double frameRate)
{
	m_frameRate = frameRate;
}

double rglGame::getFrameRate()
{
	return m_frameRate;
}

bool rglGame::run(std::string title, int width, int height, bool fullscreen, double frameRate)
{
	if (SDL_Init(SDL_INIT_EVERYTHING) < 0)
		return false;

	if ((m_pWindow = SDL_CreateWindow(title.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height,
		fullscreen ? SDL_WINDOW_FULLSCREEN : SDL_WINDOW_SHOWN)) == 0)
		return false;
	
	if ((m_pRenderer = SDL_CreateRenderer(m_pWindow, -1, 0)) == 0)
		return false;

	SDL_SetRenderDrawColor(m_pRenderer, 255, 255, 255, 255);

	m_frameRate = 1.0 / frameRate;

	m_running = true;

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
	SDL_Event event;
	if (SDL_PollEvent(&event))
	{
		switch (event.type)
		{
		case SDL_QUIT:
			m_running = false;
			break;
		}
	}
}

void rglGame::update()
{
	// TODO
}

void rglGame::render()
{
	SDL_RenderClear(m_pRenderer);

	SDL_RenderPresent(m_pRenderer);
}

void rglGame::clean()
{
	SDL_DestroyRenderer(m_pRenderer);
	SDL_DestroyWindow(m_pWindow);
	SDL_Quit();
}

// rglTextueManager

rglTextureManager* rglTextureManager::m_pInstance = 0;

rglTextureManager* rglTextureManager::getInstance()
{
	if (m_pInstance == 0)
		m_pInstance = new rglTextureManager();

	return m_pInstance;
}

bool rglTextureManager::load(std::string fileName, std::string id)
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

void rglTextureManager::draw(std::string id, int x, int y, int width, int height, SDL_RendererFlip renderFlip)
{
	drawFrame(id, x, y, width, height, 0, 0, renderFlip);
}

void rglTextureManager::drawFrame(std::string id, int x, int y, int width, int height, int currentRow, int currentFrame,
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

// rglObjectParams

rglObjectParams::rglObjectParams(int x, int y, int width, int height, std::string textureID)
	: m_x(x), m_y(y), m_width(width), m_height(height), m_textureID(textureID)
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

std::string rglObjectParams::getTextureID() const
{
	return m_textureID;
}

// rglGameActor

rglGameActor::rglGameActor(const rglObjectParams* pObjectParams)
	: rglGameObject(pObjectParams), m_position(pObjectParams->getX(), pObjectParams->getY()),
	m_velocity(0, 0), m_acceleration(0, 0)
{
	m_width = pObjectParams->getWidth();
	m_height = pObjectParams->getHeight();

	m_textureID = pObjectParams->getTextureID();

	m_currentRow = 0;
	m_currentFrame = 0;
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