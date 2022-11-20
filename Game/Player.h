#pragma once
#include "stdafx.h"

class Player
{
private:
	sf::Texture texture;
	sf::Sprite sprite;

	sf::IntRect currentFrame;

	void initTexture();
	void initSprite();
public:
	Player();

	void update();
	void updateMovement();
	void updateAnimation();
	void render(sf::RenderWindow& target);
};

