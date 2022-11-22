#pragma once
#include "stdafx.h"

class Animation
{
private:
	float deltaTime;
	sf::Sprite& sprite;
	sf::IntRect frame;

	float totalIdleTime;
	float totalRunTime;
	float totalJumpTime;
	float totalFallTime;

	const float idleSwitchTime;
	const float runSwitchTime;
	const float jumpSwitchTime;
	const float fallSwitchTime;

public:
	Animation(sf::Sprite& sprite, sf::IntRect frame);

	void setDeltaTime(float delta);
	void animate(Player_State state);
};

