#pragma once
#include "stdafx.h"

class SpriteHitBox
{
public:
	SpriteHitBox(sf::IntRect& spriteRect, sf::Vector2f hitBox);
	sf::Sprite sprite;
	sf::RectangleShape hitBox;


	void move(float x, float y);
	void move(sf::Vector2f dir);
	void setScale(float x, float y);
};

