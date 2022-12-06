#include "Tile.h"

Tile::Tile(sf::IntRect spriteRect, sf::Vector2f hitBox, sf::Texture& texture) : Entity(
	spriteRect, hitBox, texture, Origin_Pos::CENTER), isDamaging(false)
{
}
