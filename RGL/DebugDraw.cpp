#include "DebugDraw.h"
#include "Level.h"
#include "Game.h"

#include <SDL.h>

namespace rgl
{
	void DebugDraw::DrawPolygon(const b2Vec2* vertices, int32 vertexCount, const b2Color& color)
	{
		SDL_Point* points = new SDL_Point[vertexCount];

		for (int i = 0; i < vertexCount; i++)
		{
			points[i].x = m_pLevel->toLevelPositionX(m_pLevel->toPixelUnits(vertices[i].x));
			points[i].y = m_pLevel->toLevelPositionY(m_pLevel->toPixelUnits(vertices[i].y));
		}

		SDL_SetRenderDrawColor(Game::get()->getRenderer(), (int)(color.r * 255.0f), (int)(color.g * 255.0f), (int)(color.b * 255.0f), (int)(color.a * 255.0f));
		SDL_RenderDrawLines(Game::get()->getRenderer(), points, vertexCount);
		SDL_RenderDrawLine(Game::get()->getRenderer(), points[vertexCount - 1].x, points[vertexCount - 1].y, points[0].x, points[0].y);

		delete[] points;
	}

	void DebugDraw::DrawSolidPolygon(const b2Vec2* vertices, int32 vertexCount, const b2Color& color)
	{
		DrawPolygon(vertices, vertexCount, color);
	}

	void DebugDraw::DrawCircle(const b2Vec2& center, float32 radius, const b2Color& color)
	{
		SDL_Rect circleRect; // Don't question it.
		circleRect.x = m_pLevel->toLevelPositionX(m_pLevel->toPixelUnits(center.x - radius));
		circleRect.y = m_pLevel->toLevelPositionY(m_pLevel->toPixelUnits(center.y - radius));
		circleRect.w = circleRect.h = m_pLevel->toPixelUnits(radius * 2.0f);

		SDL_SetRenderDrawColor(Game::get()->getRenderer(), (int)(color.r * 255.0f), (int)(color.g * 255.0f), (int)(color.b * 255.0f), (int)(color.a * 255.0f));
		SDL_RenderDrawRect(Game::get()->getRenderer(), &circleRect);
	}

	void DebugDraw::DrawSolidCircle(const b2Vec2& center, float32 radius, const b2Vec2& axis, const b2Color& color)
	{
		DrawCircle(center, radius, color);
	}

	void DebugDraw::DrawSegment(const b2Vec2& p1, const b2Vec2& p2, const b2Color& color)
	{
		SDL_SetRenderDrawColor(Game::get()->getRenderer(), (int)(color.r * 255.0f), (int)(color.g * 255.0f), (int)(color.b * 255.0f), (int)(color.a * 255.0f));
		SDL_RenderDrawLine(Game::get()->getRenderer(),
			m_pLevel->toLevelPositionX(m_pLevel->toPixelUnits(p1.x)),
			m_pLevel->toLevelPositionY(m_pLevel->toPixelUnits(p1.y)),
			m_pLevel->toLevelPositionX(m_pLevel->toPixelUnits(p2.x)),
			m_pLevel->toLevelPositionY(m_pLevel->toPixelUnits(p2.y)));
	}
}