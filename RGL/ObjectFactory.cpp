#include "ObjectFactory.h"
#include "Debugger.h"

namespace rgl
{
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

	std::shared_ptr<GameObject> ObjectFactory::create(std::string typeID, const std::shared_ptr<ObjectParams> pObjectParams, std::string name)
	{
		std::map<std::string, std::shared_ptr<ObjectCreator>>::iterator it = m_creators.find(typeID);

		if (it == m_creators.end())
		{
			Debugger::get()->log("Type \"" + typeID + "\" has not been registered.", Debugger::WARNING);
			return 0;
		}

		if (name.empty())
			name = "(unnamed " + typeID + ")";

		return it->second->createObject(pObjectParams, name);
	}
}