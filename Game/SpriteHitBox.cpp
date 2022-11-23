#include "SpriteHitBox.h"

SpriteHitBox::SpriteHitBox(sf::IntRect spriteRect, sf::Vector2f hitBox)
{
	this->sprite = sf::Sprite();
	this->sprite.setTextureRect(spriteRect);

	this->sprite.setPosition(200, 200);

	this->hitBox.setPosition(this->sprite.getPosition().x + spriteRect.width / 2,
							 this->sprite.getPosition().y + spriteRect.height);
	this->hitBox.setOrigin(hitBox.x / 2, hitBox.y + 1);

	this->hitBox.setSize(hitBox);
	this->hitBox.setFillColor(sf::Color::Transparent);
	this->hitBox.setOutlineColor(sf::Color::Red);
	this->hitBox.setOutlineThickness(1);
}

void SpriteHitBox::move(float x, float y)
{
	sprite.move(x, y);
	hitBox.move(x, y);
}

void SpriteHitBox::setScale(float x, float y)
{
	hitBox.setScale(x, y);
	sprite.setScale(x, y);

	sprite.setPosition(hitBox.getPosition().x - sprite.getGlobalBounds().width / 2,
			           hitBox.getPosition().y - sprite.getGlobalBounds().height);
}
