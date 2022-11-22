#pragma once
#include "stdafx.h"
#include "Animation.h"

class Player
{
private:
	sf::Texture texture;
	sf::Sprite sprite;

	sf::IntRect currentFrame;
	Player_State playerState;
	Animation* animation;

	sf::Vector2f velocity;
	float veocityMax = 100.0f;
	float acceleration = 10.0f;
	float jumpVelocity = -0.35f;

	void initVariables();
	void initTexture();
	void initSprite();
public:
	Player();
	bool falling = true;

	//Accessors
	sf::Sprite getSprite();

	//Methods
	void update(float deltaTime);
	void updateMovement(float deltaTime);
	void render(sf::RenderWindow& target);
};

