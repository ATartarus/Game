#pragma once
#include "stdafx.h"

class SpriteHitBox : public sf::Drawable
{
public:
	SpriteHitBox(sf::IntRect spriteRect, sf::Vector2f hitBox);

	bool showHitBox;
	sf::Sprite sprite;
	sf::RectangleShape hitBox;

	
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
	void move(float x, float y);
	void move(sf::Vector2f dir);
	void setScale(float x, float y);
	void setPosition(float x, float y);
	void setPosition(sf::Vector2f pos);
};

