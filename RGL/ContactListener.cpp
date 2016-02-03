#include "ContactListener.h"
#include "PhysicsObject.h"

namespace rgl
{
	void ContactListener::BeginContact(b2Contact* contact)
	{
		ContactInfo contactInfo(contact);

		if (contactInfo.getObjectA() != 0)
			contactInfo.getObjectA()->onBeginContact(contactInfo.getPosition(), contactInfo.getObjectB());

		if (contactInfo.getObjectB() != 0)
			contactInfo.getObjectB()->onBeginContact(contactInfo.getPosition(), contactInfo.getObjectA());
	}

	void ContactListener::EndContact(b2Contact* contact)
	{
		ContactInfo contactInfo(contact);

		if (contactInfo.getObjectA() != 0)
			contactInfo.getObjectA()->onEndContact(contactInfo.getPosition(), contactInfo.getObjectB());

		if (contactInfo.getObjectB() != 0)
			contactInfo.getObjectB()->onEndContact(contactInfo.getPosition(), contactInfo.getObjectA());
	}
}