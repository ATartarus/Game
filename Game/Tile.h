#pragma once
#include "Entity.h"

class Tile : public Entity
{
public:
	Tile(sf::IntRect spriteRect, sf::Vector2f hitBox, sf::Texture& texture);
	bool isDamaging;
};

