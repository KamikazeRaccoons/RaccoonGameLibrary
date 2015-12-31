#pragma once

#include "stdafx.h"
#include "rglObjectParams.h"

#ifdef RGL_EXPORTS
#define RGL_GAMEOBJECT_API __declspec(dllexport)
#else
#define RGL_GAMEOBJECT_API __declspec(dllimport)
#endif

class rglGameObject
{
protected:

	rglGameObject() { }

public:

	virtual void load(const shared_ptr<rglObjectParams> pObjectParams) = 0;

	virtual void update() = 0;
	virtual void draw() = 0;
	virtual void clean() = 0;

	virtual ~rglGameObject() { }

};