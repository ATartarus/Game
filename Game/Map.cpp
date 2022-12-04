#include "Map.h"

/*  <Constructors/Destructors>  */

Map::Map(const char* map)
{
	foregroundTiles = nullptr;
	backgroundTiles = nullptr;
	objects			= nullptr;
	hitBoxesVisible = false;
	originsVisible  = false;
	viewFollow		= false;

	loadMap(map);
}

Map::~Map()
{
	deleteForegroundTiles();
	delete backgroundTiles;
	delete objects;
	delete exit;
}


void Map::deleteForegroundTiles()
{
	for (auto& vec : *foregroundTiles)
	{
		for (auto t : vec) delete t;
	}
	delete foregroundTiles;
}


/*  </Constructors/Destructors>  */



/*  <Loader methods>  */

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

	tinyxml2::XMLElement* layer = tmxMap.FirstChildElement("map");
	if (findProperty(layer, "viewFollow"))
	{
		viewFollow = findProperty(layer, "viewFollow")->FindAttribute("value")->BoolValue();
	}

	layer = layer->FirstChildElement("tileset");
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
			
			tileSheet.size.x =     tileSet->FindAttribute("columns")->IntValue();
			tileSheet.size.y =     tileSet->FindAttribute("tilecount")->IntValue() / tileSheet.size.x;
			tileSheet.tile.x =     tileSet->FindAttribute("tilewidth")->IntValue();
			tileSheet.tile.y =     tileSet->FindAttribute("tileheight")->IntValue();
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
			delete objects;
			objects = new std::vector<sf::Sprite>;
			tinyxml2::XMLElement* object = layer->FirstChildElement();
			do {
				loadObject(object);
				object = object->NextSiblingElement();
			} while (object);
		}
		else if (layerName == "layer")
		{
			std::string csv = layer->FirstChildElement("data")->FirstChild()->ToText()->Value();

			if (layer->Attribute("class") && static_cast<std::string>(layer->Attribute("class")) == "background")
			{
				delete backgroundTiles;
				backgroundTiles = new std::vector<sf::Sprite>;
				loadTiles(csv, true);
			}
			else
			{
				delete foregroundTiles;
				foregroundTiles = new std::vector<std::vector<Entity*>>(layer->FindAttribute("height")->IntValue());
				for (auto& vec : *foregroundTiles) vec = std::vector<Entity*>(layer->FindAttribute("width")->IntValue());
				m_actualBounds = sf::Vector2f((*foregroundTiles)[0].size() * tileSheet.tile.x, foregroundTiles->size() * tileSheet.tile.y);
				loadTiles(csv, false);
			}
		}
		layer = layer->NextSiblingElement();
	}
}


void Map::loadTiles(std::string& csv, bool background)
{
	std::vector<std::vector<int>> csvMap;
	std::vector<int> tmp;
	std::string tmps;
	for (auto ch : csv) {
		if (ch == '\n') {
			if (tmp.empty()) continue;
			if (!tmps.empty()) tmp.push_back(std::stoi(tmps));
			csvMap.push_back(tmp);
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


	for (int i = 0; i < csvMap.size(); i++) {
		for (int j = 0; j < csvMap[0].size(); j++) {
			if (!csvMap[i][j]) continue;

			sf::Vector2f wpos = sf::Vector2f(j * tileSheet.tile.x, i * tileSheet.tile.y);
			sf::Vector2i spos = sf::Vector2i((csvMap[i][j] % tileSheet.size.x - 1) * (tileSheet.tile.x + tileSheet.spacing.x) + tileSheet.spacing.x,
											  csvMap[i][j] / tileSheet.size.x * (tileSheet.tile.y + tileSheet.spacing.y) + tileSheet.spacing.y);

			if (background)
			{
				sf::Sprite tmp = sf::Sprite(tileSheet.texture, sf::IntRect(spos, tileSheet.tile));
				tmp.setPosition(wpos);
				backgroundTiles->push_back(tmp);
			}
			else
			{
				Entity* tmp = new Entity(sf::IntRect(spos, tileSheet.tile),
										 sf::Vector2f(tileSheet.tile.x, tileSheet.tile.y),
										 tileSheet.texture,
										 Origin_Pos::CENTER);
				tmp->setPosition(wpos.x + tileSheet.tile.x / 2.0f, wpos.y + tileSheet.tile.y / 2.0f);
				(*foregroundTiles)[i][j] = tmp;
			}
		}
	}
}


void Map::loadObject(tinyxml2::XMLElement* object)
{
	if (object->Attribute("class") && static_cast<std::string>(object->Attribute("class")) == "exit")
	{
		exit = new Exit(object->FindAttribute("x")->IntValue(),
						object->FindAttribute("y")->IntValue(),
						object->FindAttribute("width")->FloatValue(),
						object->FindAttribute("height")->FloatValue(),
						findProperty(object, "destination")->Attribute("value"));

		return;
	}

	sf::Sprite tmp = sf::Sprite();
	sf::Vector2f wpos = sf::Vector2f(object->FindAttribute("x")->FloatValue(),
									 object->FindAttribute("y")->FloatValue());
	sf::Vector2i spos = sf::Vector2i(0, 0);

	int GID = -1;
	if (object->Attribute("gid"))
	{
		GID = object->FindAttribute("gid")->IntValue();
		wpos.y -= object->FindAttribute("height")->FloatValue();
	}

	if (object->FirstChildElement("properties")) {
		GID = findProperty(object, "GID")->FindAttribute("value")->IntValue();
	}

	if (GID != -1) {
		spos = sf::Vector2i((GID % tileSheet.size.x - 1) * (tileSheet.tile.x + tileSheet.spacing.x) + tileSheet.spacing.x,
			                 GID / tileSheet.size.x * (tileSheet.tile.y + tileSheet.spacing.y) + tileSheet.spacing.y);
		tmp.setTexture(tileSheet.texture);
	}
	else tmp.setTexture(sf::Texture());

	

	tmp.setTextureRect(sf::IntRect(spos, tileSheet.tile));
	tmp.setScale(object->FindAttribute("width")->FloatValue() / tileSheet.tile.x,
		         object->FindAttribute("height")->FloatValue() / tileSheet.tile.y);
	tmp.setPosition(wpos);
	objects->push_back(tmp);
}

/*  </Loader methods>  */


tinyxml2::XMLElement* Map::findProperty(tinyxml2::XMLElement* element, std::string property)
{
	if (!element->FirstChildElement("properties")) {
		std::cout << "MAP::findProperty could not find properties" << "\n";
		return nullptr;
	}

	element = element->FirstChildElement("properties")->FirstChildElement();
	while (element) {
		if (static_cast<std::string>(element->Attribute("name")) == property) return element;
		element = element->NextSiblingElement();
	}

	std::cout << "MAP::findProperty could not find " << property << "property" << "\n";
	return nullptr;
}


sf::Vector2f Map::getActualTileSize()
{
	return (*foregroundTiles)[0][0]->getActualBounds();
}

sf::Vector2f Map::getActualBounds()
{
	return m_actualBounds;
}

/*  <Render>  */

void Map::setResolutionScale(sf::Vector2f scale)
{
	sf::Vector2f prevScale = (*foregroundTiles)[0][0]->getScale();
	sf::Vector2f prodCoeff = sf::Vector2f(scale.x / prevScale.x, scale.y / prevScale.y);
	m_actualBounds.x *= prodCoeff.x;
	m_actualBounds.y *= prodCoeff.y;

	for (auto& vec : *foregroundTiles)
	{
		for (auto t : vec)
		{
			if (t == nullptr) continue;
			t->setPosition(t->getPosition().x * prodCoeff.x, t->getPosition().y * prodCoeff.y);
			t->setScale(scale.x, scale.y);
		}
	}

	if (backgroundTiles)
	{
		prevScale = (*backgroundTiles)[0].getScale();
		for (auto& t : *backgroundTiles)
		{
			t.setPosition(t.getPosition().x * prodCoeff.x, t.getPosition().y * prodCoeff.y);
			t.setScale(scale.x, scale.y);
		}
	}


	exit->rect.left = exit->rect.left * prodCoeff.x;
	exit->rect.top = exit->rect.top * prodCoeff.y;
	exit->rect.width = exit->rect.width * prodCoeff.x;
	exit->rect.height = exit->rect.height * prodCoeff.y;
}

void Map::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	sf::Sprite back(background.texture);
	do
	{
		target.draw(back);
		back.move(back.getLocalBounds().width, 0.0f);
	} while (background.repeatX && back.getPosition().x < target.getSize().x);


	if (objects != nullptr) {
		for (auto& obj : *objects) {
			target.draw(obj);
		}
	}

	if (backgroundTiles != nullptr)
	{
		for (auto& t : *backgroundTiles) {
			target.draw(t);
		}
	}

	for (auto& vec : *foregroundTiles)
	{
		for (auto t : vec)
		{
			if (t == nullptr) continue;
			t->showHitBox = hitBoxesVisible;
			t->showOrigin = originsVisible;
			target.draw(*t);
		}
	}


	if (hitBoxesVisible)
	{
		sf::RectangleShape tmp(exit->rect.getSize());
		tmp.setPosition(exit->rect.getPosition());
		tmp.setFillColor(sf::Color::Transparent);
		tmp.setOutlineColor(sf::Color::Green);
		tmp.setOutlineThickness(1.0f);
		target.draw(tmp);
	}
}

/*  </Render>  */