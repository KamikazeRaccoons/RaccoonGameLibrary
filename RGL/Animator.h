#pragma once

#include <vector>

class Animator
{
private:

	struct Animation
	{
		int m_frameCount;
		float m_speed;

		Animation(int frameCount, float speed) : m_frameCount(frameCount), m_speed(speed) { }
	};

	std::vector<Animation> m_animations;

	int m_currentID;
	float m_currentFrame;

public:

	Animator() : m_currentID(0), m_currentFrame(0.0f) { }

	void addAnimation(int frameCount, float speed);
	void setAnimation(int id);

	void update();

	int getCurrentRow();
	int getCurrentFrame();

};

