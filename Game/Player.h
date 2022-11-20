#pragma once
#include "stdafx.h"

enum Animation_State {
	IDLE = 0,
	MOVING_LEFT,
	MOVING_RIGHT,
	JUMPING,
	FALLING,
};

class Player
{
private:
	sf::Texture texture;
	sf::Sprite sprite;

	sf::IntRect currentFrame;
	Animation_State animState;
	float totalIdleTime;
	float idleTime;
	float totalRunTime;
	float runTime;

	void initVariables();
	void initTexture();
	void initSprite();
public:
	Player();

	void update(float deltaTime);
	void updateMovement(float deltaTime);
	void updateAnimation(float deltaTime);
	void render(sf::RenderWindow& target);
};

