#include "Collider.h"
#include "Creature.h"

Collider::Collider(Creature& creature, Map& map) :
	creature(&creature), map(&map), tiles(map.foregroundTiles)
{
	tileSize = sf::Vector2f(0.0f, 0.0f);
	for (int i = 0; i < this->tiles->size() && !tileSize.x; i++) {
		for (int j = 0; j < (*this->tiles)[0].size() && !tileSize.x; j++) {
			if ((*this->tiles)[i][j] == nullptr) continue;
			tileSize = (*this->tiles)[i][j]->getActualBounds();
		}
	}

	direction = sf::Vector2i(0, 0);
}

void Collider::mapChange(Map& map)
{
	this->map = &map;
	tiles = map.foregroundTiles;
	onMapScaleChange();
}

void Collider::onMapScaleChange()
{
	tileSize = sf::Vector2f(0.0f, 0.0f);
	for (int i = 0; i < this->tiles->size() && !tileSize.x; i++) {
		for (int j = 0; j < (*this->tiles)[0].size() && !tileSize.x; j++) {
			if ((*this->tiles)[i][j] == nullptr) continue;
			tileSize = (*this->tiles)[i][j]->getActualBounds();
		}
	}
}

sf::Vector2i Collider::Check()
{
	direction = sf::Vector2i(0, 0);

	int Y1 = static_cast<int>((creature->getPosition().y - creature->getActualBounds().y) / tileSize.y);
	int Y2 = static_cast<int>(creature->getPosition().y / tileSize.y + 1);
	int X1 = static_cast<int>((creature->getPosition().x - creature->getActualBounds().x / 2.0f) / tileSize.x);
	int X2 = static_cast<int>(X1 + creature->getActualBounds().x / tileSize.x + 1);
	if (Y1 < 0) Y1 = 0;
	if (Y2 >= tiles->size()) Y2 = static_cast<int>(tiles->size()) - 1;
	if (X1 < 0) X1 = 0;
	if (X2 >= (*tiles)[0].size()) X2 = static_cast<int>((*tiles)[0].size()) - 1;

	sf::Vector2i* tmpDir = new sf::Vector2i(0, 0);
	for (int i = Y1; i <= Y2; i++) {
		for (int j = X1; j <= X2; j++) {
			if ((*tiles)[i][j] == nullptr) continue;
			calculate(*(*tiles)[i][j], *tmpDir);
			if ((*tiles)[i][j]->isDamaging && (tmpDir->x != 0 || tmpDir->y != 0))
			{
				creature->onDamageRecieve();
				direction.x = 0; direction.y = 0;
			}
			tmpDir->x = 0; tmpDir->y = 0;
		}
	}
	for (auto& slider : *map->sliders)
	{
		for (int i = slider.coreTile.y - 1; i <= slider.coreTile.y + slider.size.y; i++)
		{
			for (int j = slider.coreTile.x - 1; j <= slider.coreTile.x + slider.size.x; j++)
			{
				Tile* tile = (*tiles)[i][j];
				if (!tile) continue;
				calculate(*(*tiles)[i][j], *tmpDir);
				if ((*tiles)[i][j]->isDamaging && (tmpDir->x != 0 || tmpDir->y != 0))
				{
					creature->onDamageRecieve();
					direction.x = 0; direction.y = 0;
				}
				tmpDir->x = 0; tmpDir->y = 0;
			}
		}
	}

	delete tmpDir;

	return direction;
}

void Collider::calculate(Tile& tile, sf::Vector2i& dir)
{
	sf::Vector2f delta = sf::Vector2f(creature->getPosition().x - tile.getPosition().x,	//if > 0 player is on the RIGHT SIDE
									  creature->getPosition().y - tile.getPosition().y);	//if > 0 player is on the BOTTOM SIDE
	sf::Vector2f intersection = sf::Vector2f(fabs(delta.x) - (tile.getActualBounds().x / 2.0f + creature->getActualBounds().x / 2.0f),
							 (delta.y > 0) ? fabs(delta.y) - (tile.getActualBounds().y / 2.0f + creature->getActualBounds().y) :
											 fabs(delta.y) - tile.getActualBounds().y / 2.0f);


	sf::Vector2f shift(0.0f, 0.0f);

	if (intersection.x < 0 && intersection.y < 0 && (fabs(intersection.x - intersection.y) > 2.5f))
	{
		if (intersection.x > intersection.y)
		{
			if (delta.x > 0)
			{
				shift.x = fabs(intersection.x);
				direction.x = -1;
				dir.x = -1;
			}
			else
			{
				shift.x = intersection.x;
				direction.x = 1;
				dir.x = 1;
			}
		}
		else
		{
			if (delta.y > 0)
			{
				shift.y = fabs(intersection.y);
				direction.y = -1;
				dir.y = -1;
			}
			else
			{
				shift.y = intersection.y;
				direction.y = 1;
				dir.y = 1;
			}
		}
		if (!tile.isDamaging) creature->move(shift);
	}
}