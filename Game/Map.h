#pragma once
#include "stdafx.h"
#include "Tinyxml2/tinyxml2.h"
#include "Entity.h"

class Map : public sf::Drawable
{
private:
	struct Background {
		sf::Texture texture;
		bool repeatX;
		bool repeatY;
	} background;

	struct TileSheet {
		sf::Texture texture;
		std::string name;
		sf::Vector2i size;
		sf::Vector2i tile;
		sf::Vector2i spacing;
	} tileSheet;

	std::vector<std::vector<int>> csvTileMap;

	void loadObject(tinyxml2::XMLElement* object);
public:
	Map(const char* map);
	~Map();

	std::vector<Entity*>* tileMap;
	std::vector<sf::Sprite*>* objects;

	void loadMap(const char* map);
	void updateMap();
	void deleteMap();
	void deleteObjects();

	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
};

