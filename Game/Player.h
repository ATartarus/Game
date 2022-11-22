#pragma once
#include "stdafx.h"

enum Player_State {
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
	Player_State playerState;
	float totalIdleTime;
	float idleTime;
	float totalRunTime;
	float runTime;

	sf::Vector2f velocity;
	float veocityMax = 100.0f;
	float acceleration = 10.0f;
	float jumpHeight = 1.0f;

	void initVariables();
	void initTexture();
	void initSprite();
public:
	Player();
	bool falling = true;

	//Accessors
	sf::Sprite getSprite();

	void update(float deltaTime);
	void updateMovement(float deltaTime);
	void updateAnimation(float deltaTime);
	void render(sf::RenderWindow& target);
};

