#pragma once
#include "stdafx.h"
#include "Map.h"

class Creature;

class Collider
{
private:
	sf::Vector2i direction;
private:
	Creature* creature;
	Map* map;
	std::vector<std::vector<Tile*>>* tiles;
	sf::Vector2f tileSize;

	void calculate(Tile& tile, sf::Vector2i& direction);
public:
	Collider(Creature& creature, Map& map);

	void mapChange(Map& map);
	void onMapScaleChange();
	//Returns direction from witch player collides with the entitiy relative to the entity
	sf::Vector2i Check();
};

