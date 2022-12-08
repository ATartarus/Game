#pragma once
#include "stdafx.h"

class Animation
{
private:
	sf::Sprite& sprite;
	sf::IntRect frame;

	const float& deltaTime;

	float totalIdleTime;
	float totalRunTime;
	float totalJumpTime;
	float totalFallTime;
	float totalGuardTime;

	const float idleSwitchTime = 0.3f;
	const float runSwitchTime = 0.1f;
	const float jumpSwitchTime = 0.15f;
	const float fallSwitchTime = 0.1f;
	const float guardFrequency = 0.1f;

	void initVariables();
public:
	Animation(sf::Sprite& sprite, const float& deltaTime);

	void animate(Move_State state);

	void guardEffect();
};

