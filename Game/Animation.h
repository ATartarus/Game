#pragma once
#include "stdafx.h"
#include "SpriteHitBox.h"

class Animation
{
private:
	SpriteHitBox* entity;
	sf::Sprite& sprite;
	sf::IntRect frame;

	float deltaTime;

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
	Animation(sf::Sprite& sprite);
	Animation(SpriteHitBox* entity);

	void setDeltaTime(float delta);
	void animate(Player_State state);
	void flip(bool right);
};

