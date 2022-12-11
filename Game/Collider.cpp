#include "Collider.h"
#include "Creature.h"

Collider::Collider(Creature& creature, std::vector<std::vector<Tile*>>& tiles) :
	creature(&creature), tiles(&tiles) 
{
	tileSize = sf::Vector2f(0.0f, 0.0f);
	for (int i = 0; i < this->tiles->size() && !tileSize.x; i++) {
		for (int j = 0; j < (*this->tiles)[0].size() && !tileSize.x && (*this->tiles)[i][j]; j++) {
			tileSize = (*this->tiles)[i][j]->getActualBounds();
		}
	}
}

void Collider::mapChange(std::vector<std::vector<Tile*>>& tile)
{
	this->tiles = &tile;
	onMapScaleChange();
}

void Collider::onMapScaleChange()
{
	tileSize = sf::Vector2f(0.0f, 0.0f);
	for (int i = 0; i < this->tiles->size() && !tileSize.x; i++) {
		for (int j = 0; j < (*this->tiles)[0].size() && !tileSize.x && (*this->tiles)[i][j]; j++) {
			tileSize = (*this->tiles)[i][j]->getActualBounds();
		}
	}
}

sf::Vector2i Collider::Check()
{
	int Y1 = static_cast<int>((creature->getPosition().y - creature->getActualBounds().y) / tileSize.y);
	int Y2 = static_cast<int>(creature->getPosition().y / tileSize.y + 1);
	int X1 = static_cast<int>((creature->getPosition().x - creature->getActualBounds().x / 2.0f) / tileSize.x);
	int X2 = static_cast<int>(X1 + creature->getActualBounds().x / tileSize.x + 1);
	if (Y1 < 0) Y1 = 0;
	if (Y2 > tiles->size()) Y2 = static_cast<int>(tiles->size());
	if (X1 < 0) X1 = 0;
	if (X2 > (*tiles)[0].size()) X2 = static_cast<int>(tiles[0].size());

	sf::Vector2i direction(0, 0);

	for (int i = Y1; i < Y2; i++) {
		for (int j = X1; j < X2; j++) {
			if ((*tiles)[i][j] == nullptr) continue;
			calculate(*(*tiles)[i][j], direction);
			if ((*tiles)[i][j]->isDamaging) creature->onDamageRecieve();
		}
	}

	return direction;
}

void Collider::calculate(Tile& tile, sf::Vector2i& direction)
{
	sf::Vector2f delta = sf::Vector2f(creature->getPosition().x - tile.getPosition().x,	//if > 0 player is on the RIGHT SIDE
									  creature->getPosition().y - tile.getPosition().y);	//if > 0 player is on the BOTTOM SIDE
	sf::Vector2f intersection = sf::Vector2f(fabs(delta.x) - (tile.getActualBounds().x / 2.0f + creature->getActualBounds().x / 2.0f),
							 (delta.y > 0) ? fabs(delta.y) - (tile.getActualBounds().y / 2.0f + creature->getActualBounds().y) :
											 fabs(delta.y) - tile.getActualBounds().y / 2.0f);



	if (intersection.x < 0 && intersection.y < 0 && (fabs(intersection.x - intersection.y) > tile.getActualBounds().y / 30.0f))
	{
		if (intersection.x > intersection.y)
		{
			if (delta.x > 0)
			{
				creature->move(fabs(intersection.x), 0.0f);
				direction.x = -1;
			}
			else
			{
				creature->move(intersection.x, 0.0f);
				direction.x = 1;
			}
		}
		else
		{
			if (delta.y > 0)
			{
				creature->move(0.0f, fabs(intersection.y));
				direction.y = -1;
			}
			else
			{
				creature->move(0.0f, intersection.y);
				direction.y = 1;
			}
		}
	}

}