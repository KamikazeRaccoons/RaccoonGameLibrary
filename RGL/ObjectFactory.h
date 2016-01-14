#pragma once

#include "stdafx.h"
#include "GameObject.h"

#ifdef RGL_EXPORTS
#define RGL_OBJECTFACTORY_API __declspec(dllexport)
#else
#define RGL_OBJECTFACTORY_API __declspec(dllimport)
#endif

namespace rgl
{
	class ObjectCreator
	{
	public:

		virtual std::shared_ptr<GameObject> createObject() const = 0;
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

		static RGL_OBJECTFACTORY_API ObjectFactory* get();

		RGL_OBJECTFACTORY_API bool registerType(std::string typeID, std::shared_ptr<ObjectCreator> pCreator);
		RGL_OBJECTFACTORY_API std::shared_ptr<GameObject> create(std::string typeID);

	};
}