#include "Collider.h"

sf::Vector2i Collider::Check(SpriteHitBox& player, SpriteHitBox& tile)
{
	float deltaX = player.getPosition().x - tile.getPosition().x;	//if > 0 RIGHT SIDE
	float deltaY = player.getPosition().y - tile.getPosition().y;	//if > 0 BOTTOM SIDE
	float intersectionX = fabs(deltaX) - (tile.hitBox.getSize().x / 2 + player.getActualSize().x / 2);	//if < 0 INTERSECTION
	float intersectionY = (deltaY > 0) ? fabs(deltaY) - (tile.hitBox.getSize().y / 2 + player.getActualSize().y) :
										 fabs(deltaY) - tile.hitBox.getSize().y / 2;

	sf::Vector2i direction(0, 0);
	if (intersectionX < 0 && intersectionY < 0)
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
