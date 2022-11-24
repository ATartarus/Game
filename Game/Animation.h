#pragma once
#include "stdafx.h"

class Animation
{
private:
	float deltaTime;
	sf::Sprite& sprite;
	sf::IntRect& frame;

	float totalIdleTime;
	float totalRunTime;
	float totalJumpTime;
	float totalFallTime;

	const float idleSwitchTime = 0.3f;
	const float runSwitchTime = 0.1f;
	const float jumpSwitchTime = 0.15f;
	const float fallSwitchTime = 0.1f;

public:
	Animation(sf::Sprite& sprite, sf::IntRect& frame);

	void setDeltaTime(float delta);
	void animate(Player_State state);
};

