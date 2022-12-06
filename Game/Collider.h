#pragma once
#include "stdafx.h"
#include "Tile.h"

class Creature;

class Collider
{
private:
	Creature* creature;
	std::vector<std::vector<Tile*>>* tiles;
	sf::Vector2f tileSize;

	void calculate(Tile& tile, sf::Vector2i& direction);
public:
	Collider(Creature& creature, std::vector<std::vector<Tile*>>& tiles);

	void mapChange(std::vector<std::vector<Tile*>>& tiles);
	void mapChange();
	//Returns direction from witch player collides with the entitiy relative to the entity
	sf::Vector2i Check();
};

