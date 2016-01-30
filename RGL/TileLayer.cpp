#include "TileLayer.h"
#include "Game.h"
#include "TextureManager.h"

#include <cmath>

namespace rgl
{
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
					(id - (tileset.firstGridID - 1)) / std::max(tileset.numColumns, 1), (id - (tileset.firstGridID - 1)) % std::max(tileset.numColumns, 1));
			}
		}
	}

	void TileLayer::clean()
	{
		for (auto pBody : m_pBodies)
			m_pLevel->getWorld()->DestroyBody(pBody);

		m_pBodies.clear();
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

				b2Body* pBody = m_pLevel->getWorld()->CreateBody(&bodyDef);
				pBody->CreateFixture(&shape, 0.0f);
				m_pBodies.push_back(pBody);
			}
		}
	}
}