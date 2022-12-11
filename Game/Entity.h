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

	void setHitBox(float x, float y);
public:
	/* 
		hitBox defines size of the entity's hitbox
		texture is a reffernce to entity's texture
		textureRect defines position and size of texture's rectangle, by default textureRect covers whole texture
		position defines position of entity's origin, by defaul it is in center
	*/
	Entity(sf::Vector2f hitBox, sf::Texture& texture, sf::IntRect textureRect = sf::IntRect(0, 0, 0, 0), Origin_Pos position = Origin_Pos::DEFAULT);
	virtual ~Entity() {};
	bool showHitBox;
	bool showOrigin;


	/*  <Accessors>  */

	virtual void setScale(float x, float y);
	virtual void setScale(sf::Vector2f scale);
	sf::Vector2f getScale();

	virtual void setPosition(float x, float y);
	virtual void setPosition(sf::Vector2f pos);
	sf::Vector2f getPosition();
	//Returns hitbox bounds according to scale
	sf::Vector2f getActualBounds();

	/*  </Accessors>  */


	/*  <Methods>  */

	void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
	virtual void onWindowResize(sf::Vector2f scale) = 0;
	void move(float x, float y);
	void move(sf::Vector2f dir);
	void flip(bool right);

	/*  </Methods>  */
};

