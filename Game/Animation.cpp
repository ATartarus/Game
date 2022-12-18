#include "Animation.h"

Animation::Animation(sf::Sprite& sprite, const float& staggerTime, const float& deltaTime) :
	sprite(sprite), frame(sprite.getTextureRect()), staggerTime(staggerTime), deltaTime(deltaTime)
{
	initVariables();
}

void Animation::initVariables()
{
	totalIdleTime = -1.0f;
	totalRunTime = -1.0f;
	totalJumpTime = -1.0f;
	totalFallTime = -1.0f;
	totalGuardTime = 0.0f;
	totalInjuryTime = -1.0f;
}


void Animation::animate(Creature_State state)
{
	if ((state & Creature_State::INJURED) == Creature_State::INJURED)
	{
		if (totalInjuryTime == -1) {
			frame.left = -40;
			totalInjuryTime = injurySwitchTime;
		}
		totalInjuryTime += deltaTime;
		if (totalInjuryTime >= injurySwitchTime)
		{
			totalInjuryTime -= injurySwitchTime;
			frame.top = 250;
			frame.left += 40;
			if (frame.left >= 80) {
				frame.left = 0;
				return;
			}
		}
		totalIdleTime = -1;
		totalRunTime = -1;
		totalJumpTime = -1;
		totalFallTime = -1;
	}
	else if ((state & Creature_State::IDLE) == Creature_State::IDLE)
	{
		if (totalIdleTime == -1) {
			frame.left = -40;
			totalIdleTime = idleSwitchTime;
		}

		totalIdleTime += deltaTime;
		if (totalIdleTime >= idleSwitchTime)
		{
			totalIdleTime -= idleSwitchTime;
			frame.top = 0;
			frame.left += 40;
			if (frame.left > 160) {
				frame.left = 0;
			}
		}
		totalRunTime = -1;
		totalJumpTime = -1;
		totalFallTime = -1;
		totalInjuryTime = -1;
	}
	switch (state) {
	case Creature_State::JUMPING:
		if (totalJumpTime == -1) {
			frame.left = -40;
			totalJumpTime = jumpSwitchTime;
		}

		totalJumpTime += deltaTime;
		if (totalJumpTime >= jumpSwitchTime)
		{
			totalJumpTime -= jumpSwitchTime;
			frame.top = 100;
			frame.left += 40;
			if (frame.left > 80) {
				frame.left = 80;
			}
		}
		totalIdleTime = -1;
		totalRunTime = -1;
		totalFallTime = -1;
		totalInjuryTime = -1;
		break;
	case Creature_State::FALLING:
		if (totalFallTime == -1) {
			totalFallTime = 0;
		}

		totalFallTime += deltaTime;
		if (totalFallTime >= fallSwitchTime)
		{
			totalFallTime -= fallSwitchTime;
			frame.left = 40;
			frame.top = 200;
		}
		totalIdleTime = -1;
		totalRunTime = -1;
		totalJumpTime = -1;
		totalInjuryTime = -1;
		break;
	case Creature_State::MOVING_RIGHT:
		if (totalRunTime == -1) {
			frame.left = -40;
			totalRunTime = runSwitchTime;
		}

		totalRunTime += deltaTime;
		if (totalRunTime >= runSwitchTime)
		{
			totalRunTime -= runSwitchTime;
			frame.top = 50;
			frame.left += 40;
			if (frame.left > 360) {
				frame.left = 0;
			}
		}
		totalIdleTime = -1;
		totalJumpTime = -1;
		totalFallTime = -1;
		totalInjuryTime = -1;
		break;
	case Creature_State::MOVING_LEFT:
		if (totalRunTime == -1) {
			frame.left = -40;
			totalRunTime = runSwitchTime;
		}

		totalRunTime += deltaTime;
		if (totalRunTime >= runSwitchTime)
		{
			totalRunTime -= runSwitchTime;
			frame.top = 50;
			frame.left += 40;
			if (frame.left > 360) {
				frame.left = 0;
			}
		}
		totalIdleTime = -1;
		totalJumpTime = -1;
		totalFallTime = -1;
		totalInjuryTime = -1;
		break;
	}
	sprite.setTextureRect(frame);
}


void Animation::guardEffect()
{
	totalGuardTime += deltaTime;
	if (totalGuardTime >= guardFrequency)
	{
		totalGuardTime -= guardFrequency;
		if (sprite.getColor() == sf::Color::White)
		{
			sprite.setColor(sf::Color(255, 0, 0));
		}
		else sprite.setColor(sf::Color::White);
	}
}

