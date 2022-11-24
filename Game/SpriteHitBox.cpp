#include "SpriteHitBox.h"

SpriteHitBox::SpriteHitBox(sf::IntRect& spriteFrame, sf::Vector2f hitBox)
{
	this->sprite.setTextureRect(spriteFrame);
	this->sprite.setPosition(200.0f, 200.0f);
	this->sprite.setOrigin(spriteFrame.width / 2.0f, (float)spriteFrame.height);

	this->hitBox.setPosition(this->sprite.getPosition());
	this->hitBox.setOrigin(hitBox.x / 2.0f, hitBox.y + 1.0f);
	this->hitBox.setSize(hitBox);
	this->hitBox.setFillColor(sf::Color::Transparent);
	this->hitBox.setOutlineColor(sf::Color::Red);
	this->hitBox.setOutlineThickness(1);
}

void SpriteHitBox::move(float x, float y)
{
	if (x < 0) {
		setScale(-abs(sprite.getScale().x), sprite.getScale().y);
	}
	else if (x > 0) {
		setScale(abs(sprite.getScale().x), sprite.getScale().y);
	}
	sprite.move(x, y);
	hitBox.move(x, y);
}

void SpriteHitBox::move(sf::Vector2f dir)
{
	move(dir.x, dir.y);
}

void SpriteHitBox::setScale(float x, float y)
{
	sprite.setScale(x, y);
	hitBox.setScale(x, y);
}
