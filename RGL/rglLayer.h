#pragma once

#include "stdafx.h"

#ifdef RGL_EXPORTS
#define RGL_LAYER_API __declspec(dllexport)
#else
#define RGL_LAYER_API __declspec(dllimport)
#endif

class rglLayer
{
protected:

	virtual ~rglLayer() { }

public:

	virtual void update() = 0;
	virtual void render() = 0;

};