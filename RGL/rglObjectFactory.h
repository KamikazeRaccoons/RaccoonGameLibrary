#pragma once

#include "stdafx.h"
#include "rglGameObject.h"

#ifdef RGL_EXPORTS
#define RGL_OBJECTFACTORY_API __declspec(dllexport)
#else
#define RGL_OBJECTFACTORY_API __declspec(dllimport)
#endif

class rglObjectCreator
{
public:

	virtual shared_ptr<rglGameObject> createObject() const = 0;
	virtual ~rglObjectCreator() { }

};

class rglObjectFactory
{
private:

	static rglObjectFactory* m_pInstance;

	map<string, shared_ptr<rglObjectCreator>> m_creators;

	rglObjectFactory() { }
	~rglObjectFactory() { }

public:

	static RGL_OBJECTFACTORY_API rglObjectFactory* getInstance();

	RGL_OBJECTFACTORY_API bool registerType(string typeID, shared_ptr<rglObjectCreator> pCreator);
	RGL_OBJECTFACTORY_API shared_ptr<rglGameObject> create(string typeID);

};