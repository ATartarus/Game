#pragma once
#include "stdafx.h"

class Entity : public sf::Drawable
{
protected:
	sf::Vector2f m_position;
	sf::Vector2f m_scale;
	sf::Vector2f m_actualSize;
	sf::Sprite sprite;
	sf::RectangleShape hitBox;
public:
	Entity(sf::IntRect spriteRect, sf::Vector2f hitBox, sf::Texture& texture, Origin_Pos position = Origin_Pos::DEFAULT);
	bool showHitBox;
	bool showOrigin;


	/*  <Accessors>  */

	void setScale(float x, float y);
	sf::Vector2f getScale();

	void setPosition(float x, float y);
	void setPosition(sf::Vector2f pos);
	sf::Vector2f getPosition();

	//Returns hitbox bounds according to scale
	sf::Vector2f getActualBounds();

	/*  </Accessors>  */


	/*  <Methods>  */

	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
	void move(float x, float y);
	void move(sf::Vector2f dir);
	void flip(bool right);

	/*  </Methods>  */
};

