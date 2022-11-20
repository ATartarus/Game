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
	sf::Clock animDelay;

	void initVariables();
	void initTexture();
	void initSprite();
public:
	Player();

	void update();
	void updateMovement();
	void updateAnimation();
	void render(sf::RenderWindow& target);
};

