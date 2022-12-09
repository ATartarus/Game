#pragma once
#include "Entity.h"

class Tile : public Entity
{
public:
	Tile(sf::Vector2f hitBox, sf::Texture& texture, sf::IntRect textureRect);
	bool isDamaging;

	virtual void onWindowResize(sf::Vector2f scale) override;
};

