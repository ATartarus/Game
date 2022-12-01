#pragma once
#include "stdafx.h"
#include "Tinyxml2/tinyxml2.h"
#include "Entity.h"

class Map : public sf::Drawable
{
private:
	struct Background {
		sf::Texture texture;
		bool repeatX = false;
		bool repeatY = false;
	} background;

	struct TileSheet {
		sf::Texture texture;
		std::string name;
		sf::Vector2i size;
		sf::Vector2i tile;
		sf::Vector2i spacing;
	} tileSheet;

	struct Exit {
		const int x1;
		const int x2;
		const int y1;
		const int y2;
		const std::string nextMap;

		Exit(int x1, int x2, int y1, int y2, std::string nextMap) :
			x1(x1), x2(x2), y1(y1), y2(y2), nextMap(nextMap) {};
	};

	std::vector<sf::Sprite*>* backgroundTiles;
	std::vector<sf::Sprite*>* objects;

	void loadMap(const char* map);
	void loadTiles(std::string& csv, bool background);
	void loadObject(tinyxml2::XMLElement* object);
	void deleteForegroundTiles();
	void deleteBackgroundTiles();
	void deleteObjects();
	tinyxml2::XMLElement* findProperty(tinyxml2::XMLElement* element, std::string property);
public:
	Map(const char* map);
	~Map();

	Exit* exit;
	std::vector<Entity*>* foregroundTiles;
	bool hitBoxesVisible;
	bool originsVisible;

	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
};

