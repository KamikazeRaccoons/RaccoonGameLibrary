#pragma once

#include <Box2D\Box2D.h>

#include "PhysicsObject.h"

namespace rgl
{
	class Level;

	class ContactListener : public b2ContactListener
	{
	private:

		class ContactInfo
		{
		private:

			PhysicsObject* m_pObjectA;
			PhysicsObject* m_pObjectB;

			Vector2 m_position;

		public:

			ContactInfo(b2Contact* pContact)
			{
				void* userData = pContact->GetFixtureA()->GetBody()->GetUserData();

				m_pObjectA = userData == 0 ? 0 : static_cast<PhysicsObject*>(userData);

				userData = pContact->GetFixtureB()->GetBody()->GetUserData();

				m_pObjectB = userData == 0 ? 0 : static_cast<PhysicsObject*>(userData);

				b2WorldManifold worldManifold;
				pContact->GetWorldManifold(&worldManifold);

				int numPoints = (int)pContact->GetManifold()->pointCount;

				for (int i = 0; i < numPoints; i++)
					m_position += Vector2(worldManifold.points[i].x, worldManifold.points[i].y);

				m_position /= numPoints;
			}

			PhysicsObject* getObjectA() { return m_pObjectA; }
			PhysicsObject* getObjectB() { return m_pObjectB; }

			Vector2 getPosition() { return m_position; }
		};

	public:

		ContactListener() { }

		virtual void BeginContact(b2Contact* contact);
		virtual void EndContact(b2Contact* contact);

	};
}
