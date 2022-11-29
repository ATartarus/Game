#pragma once
#include "stdafx.h"

class SpriteHitBox : public sf::Drawable
{
private:
	sf::Vector2f m_position;
	sf::Vector2f m_scale;
	sf::Vector2f m_actualSize;
	sf::CircleShape origin;
public:
	SpriteHitBox(sf::IntRect spriteRect, sf::Vector2f hitBox, int position = DEFAULT);
	bool showHitBox;
	bool showOrigin;
	sf::Sprite sprite;
	sf::RectangleShape hitBox;


	//Accessors
	void setScale(float x, float y);
	sf::Vector2f getScale();
	void setPosition(float x, float y);
	void setPosition(sf::Vector2f pos);
	sf::Vector2f getPosition();
	sf::Vector2f getActualSize();

	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
	void move(float x, float y);
	void move(sf::Vector2f dir);
};

