#include "Tile.h"

Tile::Tile(sf::Vector2f hitBox, sf::Texture* texture, sf::IntRect textureRect) : Entity(
	hitBox, texture, textureRect, Origin_Pos::CENTER), isDamaging(false)
{
}

void Tile::onWindowResize(sf::Vector2f scale)
{
	Entity::onWindowResize(scale);
}
