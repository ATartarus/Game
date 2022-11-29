#pragma once
#include "stdafx.h"
#include "SpriteHitBox.h"

class Collider
{
public:
	static sf::Vector2i Check(SpriteHitBox& player, SpriteHitBox& tile);
};

