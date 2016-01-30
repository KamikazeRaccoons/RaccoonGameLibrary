#pragma once

#include <memory>
#include <map>

#include "ObjectParams.h"
#include "GameObject.h"

namespace rgl
{
	class ObjectCreator
	{
	public:

		virtual std::shared_ptr<GameObject> createObject(const std::shared_ptr<ObjectParams> pObjectParams, std::string name) const = 0;
		virtual ~ObjectCreator() { }

	};

	class ObjectFactory
	{
	private:

		static ObjectFactory* m_pInstance;

		std::map<std::string, std::shared_ptr<ObjectCreator>> m_creators;

		ObjectFactory() { }
		~ObjectFactory() { }

	public:

		static ObjectFactory* get();

		bool registerType(std::string typeID, std::shared_ptr<ObjectCreator> pCreator);
		std::shared_ptr<GameObject> create(std::string typeID, const std::shared_ptr<ObjectParams> pObjectParams, std::string name = std::string());

	};
}