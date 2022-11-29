#include "Map.h"

Map::Map(const char* map)
{
	tileMap = nullptr;
	loadMap(map);
}

Map::~Map()
{
	deleteMap();
}

void Map::loadMap(const char* map)
{
	std::string folder = "Maps\\";
	std::string path = folder + map;

	tinyxml2::XMLDocument tmxMap;
	if (tmxMap.LoadFile(path.c_str())) 
	{
		std::cout << "MAP::updateMap could not load tmxMap" << "\n";
		return;
	}

	std::string csvMap = tmxMap.FirstChildElement("map")->FirstChildElement("layer")->FirstChildElement("data")->FirstChild()->ToText()->Value();


	std::vector<int> tmp;
	std::string tmps;
	for (auto ch : csvMap) {
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


	if (!tilesheetName.empty() && tilesheetName == tmxMap.FirstChildElement("map")->FirstChildElement("tileset")->Attribute("source")) return;
	tilesheetName = tmxMap.FirstChildElement("map")->FirstChildElement("tileset")->Attribute("source");
	path = folder + tilesheetName;
	tinyxml2::XMLDocument tsxTileSet;

	if (tsxTileSet.LoadFile(path.c_str()))
	{
		std::cout << "MAP::updateMap could not load tsxTileSet" << "\n";
		return;
	}

	tinyxml2::XMLElement* tileSet = tsxTileSet.FirstChildElement("tileset");

	tilesheetGrid.x = tileSet->FindAttribute("columns")->IntValue();
	tilesheetGrid.y = tileSet->FindAttribute("tilecount")->IntValue() / tilesheetGrid.x;
	tile.x = tileSet->FindAttribute("tilewidth")->IntValue();
	tile.y = tileSet->FindAttribute("tileheight")->IntValue();
	spacing = 0;
	if (tileSet->Attribute("spacing")) spacing = tileSet->FindAttribute("spacing")->IntValue();
	margin = 0;
	if (tileSet->Attribute("margin")) margin = tileSet->FindAttribute("margin")->IntValue();
	

	path = folder + tileSet->FirstChildElement("image")->Attribute("source");

	if (!tilesheet.loadFromFile(path))
	{
		std::cout << "MAP::updateMap could not load tilesheet" << "\n";
		return;
	}

	updateMap();
}

void Map::updateMap()
{
	deleteMap();
	tileMap = new std::vector<Entity*>;


	for (int i = 0; i < csvTileMap.size(); i++) {
		for (int j = 0; j < csvTileMap[0].size(); j++) {
			if (!csvTileMap[i][j]) continue;

			sf::Vector2f wpos = sf::Vector2f(j * tile.x + tile.x / 2, 
											 i * tile.y + tile.y / 2);
			sf::Vector2i spos = sf::Vector2i((csvTileMap[i][j] % tilesheetGrid.x - 1) * (tile.x + spacing) + spacing,
											  csvTileMap[i][j] / tilesheetGrid.x * (tile.y + margin) + margin);
			Entity* tmp = new Entity(sf::IntRect(spos, tile), 
									 sf::Vector2f(tile.x, tile.y), 
									 tilesheet, 
									 Origin_Pos::CENTER);
			tmp->setPosition(wpos);
			tileMap->push_back(tmp);
		}
	}
}

void Map::deleteMap()
{
	if (tileMap == nullptr) return;
	for (auto it : *tileMap) delete it;
	delete tileMap;
}

void Map::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	for (auto t : *tileMap) {
		target.draw(*t);
	}
}
