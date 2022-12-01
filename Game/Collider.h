#pragma once
#include "stdafx.h"
#include "Entity.h"

class Collider
{
public:
	//Returns direction from witch player collides with the entitiy relative to the entity
	static sf::Vector2i Check(Entity& player, Entity& tile);
};

