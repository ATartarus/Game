#pragma once
#include "stdafx.h"

class Animation
{
private:
	sf::Sprite& sprite;
	sf::IntRect frame;

	float& deltaTime;

	float totalIdleTime;
	float totalRunTime;
	float totalJumpTime;
	float totalFallTime;

	const float idleSwitchTime = 0.3f;
	const float runSwitchTime = 0.1f;
	const float jumpSwitchTime = 0.15f;
	const float fallSwitchTime = 0.1f;

	void initVariables();
public:
	Animation(sf::Sprite& sprite, float& deltaTime);

	void animate(Move_State state);
};

