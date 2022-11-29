#include "Collider.h"

sf::Vector2i Collider::Check(Entity& player, Entity& tile)
{
	float deltaX = player.getPosition().x - tile.getPosition().x;	//if > 0 RIGHT SIDE
	float deltaY = player.getPosition().y - tile.getPosition().y;	//if > 0 BOTTOM SIDE
	float intersectionX = fabs(deltaX) - (tile.getActualBounds().x / 2 + player.getActualBounds().x / 2);	//if < 0 INTERSECTION
	float intersectionY = (deltaY > 0) ? fabs(deltaY) - (tile.getActualBounds().y / 2 + player.getActualBounds().y) :
										 fabs(deltaY) - tile.getActualBounds().y / 2;

	sf::Vector2i direction(0, 0);
	if (intersectionX < 0 && intersectionY < 0 && (fabs(intersectionX - intersectionY) > 1.0f))
	{
		if (intersectionX > intersectionY)
		{
			if (deltaX > 0)
			{
				player.move(fabs(intersectionX), 0.0f);
				direction.x = 1;
			}
			else
			{
				player.move(intersectionX, 0.0f);
				direction.x = -1;
			}
		}
		else
		{
			if (deltaY > 0)
			{
				player.move(0.0f, fabs(intersectionY));
				direction.y = 1;
			}
			else
			{
				player.move(0.0f, intersectionY);
				direction.y = -1;
			}
		}
	}



	return direction;
}
