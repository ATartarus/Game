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
		sf::FloatRect rect;
		const std::string nextMap;
	
		Exit(int x, int y, float width, float height, std::string nextMap) :
			rect(x, y, width, height), nextMap(nextMap) {};
	};

	std::vector<sf::Sprite>* backgroundTiles;
	std::vector<sf::Sprite>* objects;
	sf::Vector2f m_actualBounds;

	void loadMap(const char* map);
	void loadTiles(std::string& csv, bool background);
	void loadObject(tinyxml2::XMLElement* object);
	void deleteForegroundTiles();
	tinyxml2::XMLElement* findProperty(tinyxml2::XMLElement* element, std::string property);
public:
	Map(const char* map);
	~Map();

	Exit* exit;
	std::vector<std::vector<Entity*>>* foregroundTiles;
	bool hitBoxesVisible;
	bool originsVisible;
	bool viewFollow;

	sf::Vector2f getActualTileSize();
	sf::Vector2f getActualBounds();
	void setResolutionScale(sf::Vector2f scale);
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
};

