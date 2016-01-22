#pragma once

#include "stdafx.h"
#include "ObjectParams.h"
#include "GameObject.h"

namespace rgl
{
	class ObjectCreator
	{
	public:

		virtual std::shared_ptr<GameObject> createObject(std::shared_ptr<Level> pParentLevel, const std::shared_ptr<ObjectParams> pObjectParams) const = 0;
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

		static RGL_API ObjectFactory* get();

		RGL_API bool registerType(std::string typeID, std::shared_ptr<ObjectCreator> pCreator);
		RGL_API std::shared_ptr<GameObject> create(std::string typeID, std::shared_ptr<Level> pParentLevel, const std::shared_ptr<ObjectParams> pObjectParams);

	};
}