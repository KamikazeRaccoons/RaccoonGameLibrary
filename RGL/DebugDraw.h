#pragma once

#include <Box2D\Box2D.h>

#include "Debugger.h"

namespace rgl
{
	class Level;

	class DebugDraw : public b2Draw
	{
	private:

		Level* m_pLevel;

	public:

		DebugDraw(Level* pLevel) : m_pLevel(pLevel) { }

		virtual void DrawPolygon(const b2Vec2* vertices, int32 vertexCount, const b2Color& color);
		virtual void DrawSolidPolygon(const b2Vec2* vertices, int32 vertexCount, const b2Color& color);
		virtual void DrawCircle(const b2Vec2& center, float32 radius, const b2Color& color);
		virtual void DrawSolidCircle(const b2Vec2& center, float32 radius, const b2Vec2& axis, const b2Color& color);
		virtual void DrawSegment(const b2Vec2& p1, const b2Vec2& p2, const b2Color& color);
		virtual void DrawTransform(const b2Transform& xf) { }

	};
}