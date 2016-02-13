#include "Animator.h"
#include "Game.h"

void Animator::addAnimation(int frameCount, float speed)
{
	m_animations.push_back(Animation(frameCount, speed));
}

void Animator::setAnimation(int id)
{
	m_currentID = id;
	m_currentFrame = 0.0f;
}

void Animator::update()
{
	Animation currentAnimation = m_animations[m_currentID];

	m_currentFrame += currentAnimation.m_speed;

	if (m_currentFrame > currentAnimation.m_frameCount)
		m_currentFrame = 0.0f;
}

int Animator::getCurrentRow()
{
	return m_currentID;
}

int Animator::getCurrentFrame()
{
	return (int)m_currentFrame;
}