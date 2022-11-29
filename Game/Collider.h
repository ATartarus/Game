#pragma once
#include "stdafx.h"
#include "Entity.h"

class Collider
{
public:
	static sf::Vector2i Check(Entity& player, Entity& tile);
};

