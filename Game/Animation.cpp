#include "Animation.h"

Animation::Animation(sf::Sprite& sprite) :
	sprite(sprite)
{
	initVariables();
}

Animation::Animation(SpriteHitBox& entity) :
	sprite(entity.sprite)
{
	initVariables();
}


void Animation::initVariables()
{
	frame = sprite.getTextureRect();
	deltaTime = 0.1f;

	totalIdleTime = -1.0f;
	totalRunTime = -1.0f;
	totalJumpTime = -1.0f;
	totalFallTime = -1.0f;
}


void Animation::setDeltaTime(float delta)
{
	deltaTime = delta;
}

void Animation::animate(Player_State state)
{
	switch (state) {
	case IDLE:
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

			sprite.setTextureRect(frame);
		}
		totalRunTime = -1;
		totalJumpTime = -1;
		totalFallTime = -1;
		break;
	case FALLING:
		if (totalFallTime == -1) {
			totalFallTime = 0;
		}

		totalFallTime += deltaTime;
		if (totalFallTime >= fallSwitchTime) 
		{
			totalFallTime -= fallSwitchTime;
			frame.left = 40;
			frame.top = 200;
			sprite.setTextureRect(frame);
		}

		totalIdleTime = -1;
		totalRunTime = -1;
		totalJumpTime = -1;
		break;
	case MOVING_RIGHT:
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

			sprite.setTextureRect(frame);
		}
		totalIdleTime = -1;
		totalJumpTime = -1;
		totalFallTime = -1;
		break;
	case MOVING_LEFT:
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

			sprite.setTextureRect(frame);
		}
		totalIdleTime = -1;
		totalJumpTime = -1;
		totalFallTime = -1;
		break;
	case JUMPING:
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
				frame.left = 0;
			}
			sprite.setTextureRect(frame);
		}

		totalIdleTime = -1;
		totalRunTime = -1;
		totalFallTime = -1;
		break;
	}
}

