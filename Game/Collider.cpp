#include "Collider.h"

sf::Vector2i Collider::Check(Entity& player, Entity& tile)
{
	sf::Vector2f delta = sf::Vector2f(player.getPosition().x - tile.getPosition().x,	//if > 0 player is on the RIGHT SIDE
									  player.getPosition().y - tile.getPosition().y);	//if > 0 player is on the BOTTOM SIDE
	sf::Vector2f intersection = sf::Vector2f(fabs(delta.x) - (tile.getActualBounds().x / 2.0f + player.getActualBounds().x / 2.0f), 
							 (delta.y > 0) ? fabs(delta.y) - (tile.getActualBounds().y / 2.0f + player.getActualBounds().y) :
										     fabs(delta.y) - tile.getActualBounds().y / 2.0f);

	
	sf::Vector2i direction(0, 0);
	if (intersection.x < 0 && intersection.y < 0 && (fabs(intersection.x - intersection.y) > tile.getActualBounds().y / 30.0f))
	{
		if (intersection.x > intersection.y)
		{
			if (delta.x > 0)
			{
				player.move(fabs(intersection.x), 0.0f);
				direction.x = -1;
			}
			else
			{
				player.move(intersection.x, 0.0f);
				direction.x = 1;
			}
		}
		else
		{
			if (delta.y > 0)
			{
				player.move(0.0f, fabs(intersection.y));
				direction.y = -1;
			}
			else
			{
				player.move(0.0f, intersection.y);
				direction.y = 1;
			}
		}
	}



	return direction;
}
