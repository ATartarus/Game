#include "Map.h"


Map::Map(const char* map)
{
	background.repeatX = false;
	background.repeatY = false;
	tileMap = nullptr;
	objects = nullptr;
	loadMap(map);
}

Map::~Map()
{
	deleteMap();
	deleteObjects();
}

void Map::loadMap(const char* map)
{
	const std::string folder = "Maps\\";
	std::string PATH = folder + map;

	tinyxml2::XMLDocument tmxMap;
	if (tmxMap.LoadFile(PATH.c_str())) 
	{
		std::cout << "MAP::loadMap could not load tmxMap" << "\n";
		return;
	}

	tinyxml2::XMLElement* layer = tmxMap.FirstChildElement("map")->FirstChildElement();
	while (layer)
	{
		std::string layerName = layer->Name();

		if (layerName == "tileset")
		{
			tinyxml2::XMLDocument tsxTileSet;

			if (tileSheet.name == layer->Attribute("source")) return;
			tileSheet.name = layer->Attribute("source");

			PATH = folder + tileSheet.name;
			if (tsxTileSet.LoadFile(PATH.c_str())) {
				std::cout << "MAP::loadMap could not load tsxTileSet" << "\n";
				return;
			}

			tinyxml2::XMLElement* tileSet = tsxTileSet.FirstChildElement("tileset");
			
			tileSheet.size.x = tileSet->FindAttribute("columns")->IntValue();
			tileSheet.size.y = tileSet->FindAttribute("tilecount")->IntValue() / tileSheet.size.x;
			tileSheet.tile.x = tileSet->FindAttribute("tilewidth")->IntValue();
			tileSheet.tile.y = tileSet->FindAttribute("tileheight")->IntValue();
			tileSheet.spacing.x = (tileSet->Attribute("spacing")) ? tileSet->FindAttribute("spacing")->IntValue() : 0;
			tileSheet.spacing.y = (tileSet->Attribute("margin")) ? tileSet->FindAttribute("margin")->IntValue() : 0;

			PATH = folder + tileSet->FirstChildElement("image")->Attribute("source");
			if (!tileSheet.texture.loadFromFile(PATH)){
				std::cout << "MAP::loadMap could not load tilesheet" << "\n";
				return;
			}
		}
		else if (layerName == "imagelayer")
		{
			background.repeatX = layer->Attribute("repeatx");
			background.repeatY = layer->Attribute("repeaty");

			PATH = folder + layer->FirstChildElement("image")->Attribute("source");
			if (!background.texture.loadFromFile(PATH)){
				std::cout << "MAP::loadMap could not load background image" << "\n";
			}
		}
		else if (layerName == "objectgroup")
		{
			deleteObjects();
			objects = new std::vector<sf::Sprite*>;
			tinyxml2::XMLElement* object = layer->FirstChildElement();
			do {
				loadObject(object);
				object = object->NextSiblingElement();
			} while (object);
		}
		else if (layerName == "layer")
		{
			std::string csv = layer->FirstChildElement("data")->FirstChild()->ToText()->Value();
			std::vector<int> tmp;
			std::string tmps;
			for (auto ch : csv) {
				if (ch == '\n') {
					if (tmp.empty()) continue;
					if (!tmps.empty()) tmp.push_back(std::stoi(tmps));
					csvTileMap.push_back(tmp);
					tmp.clear();
					tmps.clear();
				}
				else if (ch == ',') {
					tmp.push_back(std::stoi(tmps));
					tmps.clear();
					continue;
				}
				tmps += ch;
			}

			updateMap();
		}
		layer = layer->NextSiblingElement();
	}
}

void Map::updateMap()
{
	deleteMap();
	tileMap = new std::vector<Entity*>;


	for (int i = 0; i < csvTileMap.size(); i++) {
		for (int j = 0; j < csvTileMap[0].size(); j++) {
			if (!csvTileMap[i][j]) continue;

			sf::Vector2f wpos = sf::Vector2f(j * tileSheet.tile.x + tileSheet.tile.x / 2, 
											 i * tileSheet.tile.y + tileSheet.tile.y / 2);
			sf::Vector2i spos = sf::Vector2i((csvTileMap[i][j] % tileSheet.size.x - 1) * (tileSheet.tile.x + tileSheet.spacing.x) + tileSheet.spacing.x,
											  csvTileMap[i][j] / tileSheet.size.x * (tileSheet.tile.y + tileSheet.spacing.y) + tileSheet.spacing.y);
			Entity* tmp = new Entity(sf::IntRect(spos, tileSheet.tile), 
									 sf::Vector2f(tileSheet.tile.x, tileSheet.tile.y), 
									 tileSheet.texture, 
									 Origin_Pos::CENTER);
			tmp->setPosition(wpos);
			tileMap->push_back(tmp);
		}
	}
}

void Map::loadObject(tinyxml2::XMLElement* object)
{
	int GID = -1;
	if (object->FirstChildElement("properties")) {
		tinyxml2::XMLElement* prop = object->FirstChildElement("properties")->FirstChildElement();
		do {
			if (static_cast<std::string>(prop->Attribute("name")) == "GID") GID = prop->FindAttribute("value")->IntValue();
			prop = prop->NextSiblingElement();
		} while (prop);
	}

	sf::Sprite* tmp = new sf::Sprite();
	sf::Vector2f wpos = sf::Vector2f(object->FindAttribute("x")->FloatValue(),
									 object->FindAttribute("y")->FloatValue());
	sf::Vector2i spos = sf::Vector2i(0, 0);
	if (GID != -1) {
		spos = sf::Vector2i((GID % tileSheet.size.x - 1) * (tileSheet.tile.x + tileSheet.spacing.x) + tileSheet.spacing.x,
		                     GID / tileSheet.size.x * (tileSheet.tile.y + tileSheet.spacing.y) + tileSheet.spacing.y);
		tmp->setTexture(tileSheet.texture);
	}


	std::string name = object->Attribute("name");
	if (name == "rectangle")
	{
		tmp->setTextureRect(sf::IntRect(spos, tileSheet.tile));
		tmp->setScale(object->FindAttribute("width")->FloatValue() / tileSheet.tile.x,
		           	  object->FindAttribute("height")->FloatValue() / tileSheet.tile.y);
		tmp->setPosition(wpos);
		objects->push_back(tmp);
	}
}

void Map::deleteMap()
{
	if (tileMap == nullptr) return;
	for (auto it : *tileMap) delete it;
	delete tileMap;
}

void Map::deleteObjects()
{
	if (objects == nullptr) return;
	for (auto it : *objects) delete it;
	delete objects;
}

void Map::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	sf::Sprite back(background.texture);
	do
	{
		target.draw(back);
		back.move(back.getLocalBounds().width, 0.0f);
	} while (background.repeatX && back.getPosition().x < target.getSize().x);

	for (auto obj : *objects) {
		target.draw(*obj);
	}

	for (auto t : *tileMap) {
		target.draw(*t);
	}
}
