#pragma once
#include "stdafx.h"
#include "Tinyxml2/tinyxml2.h"
#include "SpriteHitBox.h"

class Map : public sf::Drawable
{
private:
	sf::Texture tilesheet;
	sf::Vector2i tilesheetGrid;
	std::string tilesheetName;
	std::vector<std::vector<int>> csvTileMap;
	sf::Vector2i tile;
	int spacing;
	int margin;
public:
	Map(const char* map);
	~Map();

	std::vector<SpriteHitBox*>* tileMap;

	void loadMap(const char* map);
	void updateMap();
	void deleteMap();

	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
};

