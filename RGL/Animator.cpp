#include "Animator.h"

void Animator::setAnimation(int frameCount, float speed)
{
	m_frameCount = frameCount;
	m_speed = speed;
	m_currentFrame = 0.0f;
}

void Animator::update()
{
	m_currentFrame += m_speed;

	if (m_currentFrame > m_frameCount)
		m_currentFrame = 0.0f;
}

int Animator::getCurrentFrame()
{
	return (int)m_currentFrame;
}