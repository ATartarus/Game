#pragma once
#include "stdafx.h"

class Animation
{
private:
	sf::Sprite& sprite;
	sf::IntRect frame;

	const float& deltaTime;
	const float& staggerTime;

	float totalIdleTime;
	float totalRunTime;
	float totalJumpTime;
	float totalFallTime;
	float totalGuardTime;
	float totalInjuryTime;

	const float idleSwitchTime = 0.3f;
	const float runSwitchTime = 0.1f;
	const float jumpSwitchTime = 0.15f;
	const float fallSwitchTime = 0.1f;
	const float guardFrequency = 0.1f;
	const float injurySwitchTime = staggerTime / 2 * 0.8;

	void initVariables();
public:
	Animation(sf::Sprite& sprite, const float& staggerTime, const float& deltaTime);

	void animate(Creature_State state);

	void guardEffect();
};

