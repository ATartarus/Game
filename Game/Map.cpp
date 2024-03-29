#include "Map.h"

/*  <Constructors/Destructors>  */

Map::Map(const char* map) :
	name(map)
{
	foregroundTiles  = nullptr;
	backgroundLayers = nullptr;
	objects		     = nullptr;
	exits			 = nullptr;
	hitBoxesVisible  = false;
	originsVisible   = false;
	viewFollow		 = false;
	m_scale			 = sf::Vector2f(1.0f, 1.0f);

	loadMap(map);
}

Map::~Map()
{
	deleteForegroundTiles();
	deleteBackgroundLayers();
	delete objects;
	delete exits;
	delete sliders;
}


void Map::deleteForegroundTiles()
{
	if (foregroundTiles == nullptr) return;
	for (auto& vec : *foregroundTiles)
	{
		for (auto t : vec) delete t;
	}
	delete foregroundTiles;
}

void Map::deleteBackgroundLayers()
{
	if (backgroundLayers == nullptr) return;
	for (auto& layer : *backgroundLayers)
	{
		for (auto& vec : layer.matrix)
		{
			for (auto t : vec) delete t;
		}
	}
	delete backgroundLayers;
}

/*  </Constructors/Destructors>  */


/*#################################################################################################################################################*/


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
			
			tileSheet.size.x    =  tileSet->FindAttribute("columns")->IntValue();
			tileSheet.size.y    =  tileSet->FindAttribute("tilecount")->IntValue() / tileSheet.size.x;
			tileSheet.tile.x    =  tileSet->FindAttribute("tilewidth")->IntValue();
			tileSheet.tile.y    =  tileSet->FindAttribute("tileheight")->IntValue();
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
			backgroundImage.repeatX = layer->Attribute("repeatx");
			backgroundImage.repeatY = layer->Attribute("repeaty");
			backgroundImage.parallax.x = (layer->Attribute("parallaxx")) ? layer->FindAttribute("parallaxx")->FloatValue() : 1;
			backgroundImage.parallax.y = (layer->Attribute("parallaxy")) ? layer->FindAttribute("parallaxy")->FloatValue() : 1;

			PATH = folder + layer->FirstChildElement("image")->Attribute("source");
			if (!backgroundImage.texture.loadFromFile(PATH)){
				std::cout << "MAP::loadMap could not load background image" << "\n";
			}

			backgroundImage.sprite.setTexture(backgroundImage.texture);
			float scale = (findProperty(layer, "scale")) ? findProperty(layer, "scale")->FindAttribute("value")->FloatValue() : 1;
			backgroundImage.localScale = sf::Vector2f(scale, scale);
			backgroundImage.sprite.setScale(scale, scale);
		}
		else if (layerName == "objectgroup")
		{
			delete objects;
			objects = new std::vector<sf::Sprite>;
			exits = new std::vector<Exit>;
			sliders = new std::vector<Slider>;
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
				if (backgroundLayers == nullptr) backgroundLayers = new std::vector<BackgroundLayer>;
				backgroundLayers->push_back(BackgroundLayer());
				parseColor(backgroundLayers->back().tintColor, layer->Attribute("tintcolor"));
				backgroundLayers->back().matrix.resize(layer->FindAttribute("height")->IntValue());
				for (auto& vec : backgroundLayers->back().matrix) vec = std::vector<sf::Sprite*>(layer->FindAttribute("width")->IntValue());
				loadTiles(csv, true);
			}
			else
			{
				deleteForegroundTiles();
				foregroundTiles = new std::vector<std::vector<Tile*>>(layer->FindAttribute("height")->IntValue());
				for (auto& vec : *foregroundTiles) vec = std::vector<Tile*>(layer->FindAttribute("width")->IntValue());
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
			sf::Vector2i spos;

			int mod = csvMap[i][j] % tileSheet.size.x;
			int div = csvMap[i][j] / tileSheet.size.x;
			if (!mod) spos = sf::Vector2i(tileSheet.size.x - 1, div - 1);
			else	  spos = sf::Vector2i(mod - 1, div);

			spos.x = spos.x * (tileSheet.tile.x + tileSheet.spacing.x) + tileSheet.spacing.x;
			spos.y = spos.y * (tileSheet.tile.y + tileSheet.spacing.y) + tileSheet.spacing.y;


			if (background)
			{
				sf::Sprite* tmp = new sf::Sprite(tileSheet.texture, sf::IntRect(spos, tileSheet.tile));
				tmp->setPosition(wpos);
				if (backgroundLayers->back().tintColor != sf::Color()) tmp->setColor(backgroundLayers->back().tintColor);
				backgroundLayers->back().matrix[i][j] = tmp;
			}
			else
			{
				sf::Vector2f hitBox(tileSheet.tile.x, tileSheet.tile.y);
				wpos.x += tileSheet.tile.x / 2.0f;
				wpos.y += tileSheet.tile.y / 2.0f;
				bool damaging = false;


				if (csvMap[i][j] >= 22 && csvMap[i][j] <= 25) {
					damaging = true;
					if (csvMap[i][j] == 22 || csvMap[i][j] == 24) {
						hitBox.y /= 3.0f;
					}
					if (csvMap[i][j] == 23 || csvMap[i][j] == 25) {
						hitBox.x /= 3.0f;
					}
				}

				Tile* tmp = new Tile(hitBox,
					&tileSheet.texture,
					sf::IntRect(spos, tileSheet.tile));
				tmp->setPosition(wpos);
				tmp->isDamaging = damaging;
				(*foregroundTiles)[i][j] = tmp;
			}
		}
	}
}


void Map::loadObject(tinyxml2::XMLElement* object)
{
	std::string objClass;

	if (object->Attribute("class"))
	{
		objClass = static_cast<std::string>(object->Attribute("class"));
		if (objClass == "exit")
		{
			Exit tmp = Exit(object->FindAttribute("x")->IntValue(),
				object->FindAttribute("y")->IntValue(),
				object->FindAttribute("width")->FloatValue(),
				object->FindAttribute("height")->FloatValue(),
				findProperty(object, "destination")->Attribute("value"));
			exits->push_back(tmp);
		}
		else if (objClass == "slider")
		{
			Slider tmp{};
			object->QueryAttribute("x", &tmp.ltEdge.x);
			object->QueryAttribute("y", &tmp.ltEdge.y);
			tmp.coreTile.x = tmp.ltEdge.x / tileSheet.tile.x;
			tmp.coreTile.y = tmp.ltEdge.y / tileSheet.tile.y;

			object->QueryAttribute("width", &tmp.size.x);
			object->QueryAttribute("height", &tmp.size.y);
			tmp.size.x = tmp.size.x / tileSheet.tile.x;
			tmp.size.y = tmp.size.y / tileSheet.tile.y;

			findProperty(object, "stopX")->QueryAttribute("value", &tmp.rbEdge.x);
			findProperty(object, "stopY")->QueryAttribute("value", &tmp.rbEdge.y);
			findProperty(object, "velocityX")->QueryAttribute("value", &tmp.velocity.x);
			findProperty(object, "velocityY")->QueryAttribute("value", &tmp.velocity.y);

			sf::Vector2f swap{};
			if (tmp.velocity.x < 0) {
				swap = tmp.ltEdge;
				tmp.ltEdge.x = tmp.rbEdge.x;
				tmp.rbEdge.x = swap.x;
			}
			if (tmp.velocity.y < 0) {
				swap = tmp.ltEdge;
				tmp.ltEdge.y = tmp.rbEdge.y;
				tmp.rbEdge.y = swap.y;
			}

			tmp.ltEdge.x += tileSheet.tile.x / 2.0f;
			tmp.ltEdge.y += tileSheet.tile.y / 2.0f;
			tmp.rbEdge.x -= tileSheet.tile.x / 2.0f;
			tmp.rbEdge.y -= tileSheet.tile.y / 2.0f;

			sliders->push_back(tmp);
		}
	}
	//implementation of non-active sprite objects, need to reconsider this

	/*
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
	*/
}

/*  </Loader methods>  */


/*#################################################################################################################################################*/


/*  <Supplemental XML loader methods>  */

tinyxml2::XMLElement* Map::findProperty(tinyxml2::XMLElement* element, std::string property)
{
	if (!element->FirstChildElement("properties")) {
		std::cout << "MAP::findProperty could not find properties in " << element->Name() << "\n";
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

bool Map::parseColor(sf::Color& destination, const char* color)
{
	if (color == NULL) {
		std::cout << "MAP::parseColor color is empty" << "\n";
		return false;
	}
	sf::Uint8 r = 16 * ((color[1] > 57) ? color[1] - 87 : color[1] - 48);
	r += (color[2] > 57) ? color[2] - 87 : color[2] - 48;
	sf::Uint8 g = 16 * ((color[3] > 57) ? color[3] - 87 : color[3] - 48);
	g += (color[4] > 57) ? color[4] - 87 : color[4] - 48;
	sf::Uint8 b = 16 * ((color[5] > 57) ? color[5] - 87 : color[5] - 48);
	b += (color[6] > 57) ? color[6] - 87 : color[6] - 48;
	destination = sf::Color(r, g, b);
	return true;
}

/*  </Supplemental XML loader methods>  */


/*#################################################################################################################################################*/


/*  <Accessors>  */

void Map::update(float dt)
{
	for (auto& slider : *sliders)
	{
		auto sliderMove = [this, &slider, dt](sf::Vector2f velocity)
		{
			for (int i = slider.coreTile.y - 1; i <= slider.coreTile.y + slider.size.y; i++)
			{
				for (int j = slider.coreTile.x - 1; j <= slider.coreTile.x + slider.size.x; j++)
				{
					Tile* tile = (*foregroundTiles)[i][j];
					if (!tile) continue;
					sf::Vector2f shift = velocity * dt;

					tile->move(shift);
				}
			}
		};

		sliderMove(slider.velocity);
		Tile* tile = (*foregroundTiles)[slider.coreTile.y][slider.coreTile.x];

		sf::Vector2f tilePos = tile->getPosition();
		sf::Vector2f shift{};

		const float eps = 0.00001;
		if (slider.ltEdge.x - tilePos.x > eps)
			shift.x = slider.ltEdge.x - tilePos.x;
		else if (slider.rbEdge.x - tilePos.x < eps)
			shift.x = slider.rbEdge.x - tilePos.x;

		if (slider.ltEdge.y - tilePos.y > eps)
			shift.y = slider.ltEdge.y - tilePos.y;
		else if (slider.rbEdge.y - tilePos.y < eps)
			shift.y = slider.rbEdge.y - tilePos.y;

		if (shift.x != 0.0f) {
			slider.velocity.x = -slider.velocity.x;
			sliderMove(shift);
		}
		if (shift.y != 0.0f) {
			slider.velocity.y = -slider.velocity.y;
			sliderMove(shift);
		}
	}
}

sf::Vector2f Map::getScale() const
{
	return m_scale;
}

sf::Vector2f Map::getActualTileSize() const
{
	sf::Vector2f res(0.0f, 0.0f);
	for (int i = 0; i < foregroundTiles->size() && !res.x; i++) {
		for (int j = 0; j < (*foregroundTiles)[0].size() && !res.x; j++) {
			if ((*foregroundTiles)[i][j] == nullptr) continue;
			res = (*foregroundTiles)[i][j]->getActualBounds();
		}
	}
	return res;
}

sf::Vector2f Map::getActualBounds() const
{
	return m_actualBounds;
}

/*  </Accessors>  */


/*#################################################################################################################################################*/


/*  <Render>  */

void Map::onWindowResize(sf::Vector2f scale)
{
	sf::Vector2f prodCoeff = sf::Vector2f(scale.x / m_scale.x, scale.y / m_scale.y);
	m_actualBounds.x *= prodCoeff.x;
	m_actualBounds.y *= prodCoeff.y;

	for (auto& vec : *foregroundTiles)
	{
		for (auto t : vec)
		{
			if (t == nullptr) continue;
			t->onWindowResize(scale);
		}
	}

	if (backgroundLayers) {
		for (auto& layer : *backgroundLayers) {
			for (auto& vec : layer.matrix) {
				for (auto t : vec) {
					if (t == nullptr) continue;
					t->setPosition(t->getPosition().x * prodCoeff.x, t->getPosition().y * prodCoeff.y);
					t->setScale(scale.x, scale.y);
				}
			}
		}
	}

	backgroundImage.sprite.setScale(backgroundImage.localScale.x * scale.x, backgroundImage.localScale.y * scale.y);
	backgroundImage.sprite.setPosition(backgroundImage.sprite.getPosition().x * prodCoeff.x, backgroundImage.sprite.getPosition().y * prodCoeff.y);

	for (auto& exit : *exits) {
		exit.rect.left = exit.rect.left * prodCoeff.x;
		exit.rect.top = exit.rect.top * prodCoeff.y;
		exit.rect.width = exit.rect.width * prodCoeff.x;
		exit.rect.height = exit.rect.height * prodCoeff.y;
	}

	for (auto& slider : *sliders) {
		slider.ltEdge.x *= prodCoeff.x;
		slider.ltEdge.y *= prodCoeff.y;
		slider.rbEdge.x *= prodCoeff.x;
		slider.rbEdge.y *= prodCoeff.y;
		slider.velocity.x *= prodCoeff.x;
		slider.velocity.y *= prodCoeff.y;
	}

	m_scale = scale;
}


void Map::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	sf::Sprite backCpy = backgroundImage.sprite;
	backCpy.setScale(backgroundImage.sprite.getScale());
	do
	{
		target.draw(backCpy);
		backCpy.move(backCpy.getGlobalBounds().width, 0.0f);
	} while (backgroundImage.repeatX && backCpy.getPosition().x < target.getView().getCenter().x + target.getSize().x / 2.0f);

	if (objects != nullptr) {
		for (auto& obj : *objects) {
			target.draw(obj);
		}
	}

	
	int Y1 = static_cast<int>((target.getView().getCenter().y - target.getView().getSize().y / 2.0f) / getActualTileSize().y);
	int Y2 = static_cast<int>(Y1 + target.getSize().y / getActualTileSize().y + 2);
	int X1 = static_cast<int>((target.getView().getCenter().x - target.getView().getSize().x / 2.0f) / getActualTileSize().x);
	int X2 = static_cast<int>(X1 + target.getSize().x / getActualTileSize().x + 2);

	if (Y1 < 0) Y1 = 0;
	if (Y2 > foregroundTiles->size()) Y2 = static_cast<int>(foregroundTiles->size());
	if (X1 < 0) X1 = 0;
	if (X2 > (*foregroundTiles)[0].size()) X2 = static_cast<int>((*foregroundTiles)[0].size());

	if (backgroundLayers) {
		for (auto& layer : *backgroundLayers) {
			for (int i = Y1; i < Y2; i++) {
				for (int j = X1; j < X2; j++) {
					if (layer.matrix[i][j])
					{
						target.draw(*layer.matrix[i][j]);
					}
				}
			}
		}
	}


	for (int i = Y1; i < Y2; i++) {
		for (int j = X1; j < X2; j++) {
			if ((*foregroundTiles)[i][j])
			{
				(*foregroundTiles)[i][j]->showHitBox = hitBoxesVisible;
				(*foregroundTiles)[i][j]->showOrigin = originsVisible;
				target.draw(*(*foregroundTiles)[i][j]);
			}
		}
	}

	for (auto& slider : *sliders)
	{
		for (int i = slider.coreTile.y - 1; i <= slider.coreTile.y + slider.size.y; i++)
		{
			for (int j = slider.coreTile.x - 1; j <= slider.coreTile.x + slider.size.x; j++)
			{
				Tile* tile = (*foregroundTiles)[i][j];
				if (!tile) continue;
				tile->draw(target, states);
			}
		}
	}


	if (hitBoxesVisible)
	{
		for (auto& exit : *exits) {
			sf::RectangleShape tmp(exit.rect.getSize());
			tmp.setPosition(exit.rect.getPosition());
			tmp.setFillColor(sf::Color::Transparent);
			tmp.setOutlineColor(sf::Color::Green);
			tmp.setOutlineThickness(1.0f);
			target.draw(tmp);
		}
	}
}

/*  </Render>  */