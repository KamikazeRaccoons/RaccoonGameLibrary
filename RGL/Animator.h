#pragma once

#include <vector>

class Animator
{
private:

	float m_speed;
	int m_frameCount;
	float m_currentFrame;

public:

	Animator() : m_speed(0.0f), m_frameCount(0), m_currentFrame(0.0f) { }

	void setAnimation(int frameCount, float speed);
	
	void update();

	int getCurrentFrame();

};

