#pragma once
#include "stdafx.h"
#include "Tinyxml2/tinyxml2.h"
#include "Entity.h"

class Map : public sf::Drawable
{
private:
	sf::Vector2f m_actualBounds;
	std::vector<sf::Sprite>* objects;
private:
	struct Image {
		sf::Texture texture;
		sf::Sprite sprite;
		sf::Vector2f localScale;
		sf::Vector2f parallax;
		bool repeatX = false;
		bool repeatY = false;
	} backgroundImage;

	struct Tiles {
		std::vector<std::vector<sf::Sprite*>>* matrix{};
		sf::Color tintColor;
	} backgroundTiles;

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



	void loadMap(const char* map);
	void loadTiles(std::string& csv, bool background);
	void loadObject(tinyxml2::XMLElement* object);
	void deleteForegroundTiles();
	void deleteBackgroundTiles();
	tinyxml2::XMLElement* findProperty(tinyxml2::XMLElement* element, std::string property);
	bool parseColor(sf::Color& destination, const char* color);

public:
	Map(const char* map);
	~Map();

	Exit* exit;
	std::vector<std::vector<Entity*>>* foregroundTiles;
	bool hitBoxesVisible;
	bool originsVisible;
	bool viewFollow;


	sf::Vector2f getActualTileSize() const;
	sf::Vector2f getActualBounds() const;
	void setResolutionScale(sf::Vector2f scale);
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
};

