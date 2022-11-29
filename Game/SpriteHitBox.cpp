#include "SpriteHitBox.h"

SpriteHitBox::SpriteHitBox(sf::IntRect spriteFrame, sf::Vector2f hitBox, int position)
{
	this->sprite.setTextureRect(spriteFrame);

	if (position == (BOTTOM | CENTER)) {
		this->sprite.setOrigin(spriteFrame.width / 2.0f, (float)spriteFrame.height);
		this->hitBox.setOrigin(hitBox.x / 2.0f, hitBox.y);
	}
	else if (position == CENTER) {
		this->sprite.setOrigin(spriteFrame.width / 2.0f, spriteFrame.height / 2.0f);
		this->hitBox.setOrigin(hitBox.x / 2.0f, hitBox.y / 2.0f);
	}

	
	this->hitBox.setSize(hitBox);
	this->hitBox.setFillColor(sf::Color::Transparent);
	this->hitBox.setOutlineColor(sf::Color::Red);
	this->hitBox.setOutlineThickness(1);

	this->origin.setFillColor(sf::Color::Green);
	this->origin.setRadius(3.0f);
	this->origin.setOrigin(3.0f, 3.0f);

	this->showHitBox = false;
	this->showOrigin = false;

	m_position = this->hitBox.getPosition();
	m_scale = sf::Vector2f(1.0f, 1.0f);
	m_actualSize = hitBox;
}

void SpriteHitBox::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	target.draw(sprite);
	if (showHitBox) target.draw(hitBox);
	if (showOrigin) target.draw(origin);
}

void SpriteHitBox::move(float x, float y)
{
	m_position.x += x;
	m_position.y += y;
	sprite.move(x, y);
	hitBox.move(x, y);
	origin.move(x, y);
}

void SpriteHitBox::move(sf::Vector2f dir)
{
	move(dir.x, dir.y);
}

void SpriteHitBox::setScale(float x, float y)
{
	m_actualSize = sf::Vector2f(m_actualSize.x / fabs(m_scale.x) * fabs(x), m_actualSize.y / fabs(m_scale.y) * fabs(y));
	m_scale = sf::Vector2f(x, y);
	sprite.setScale(x, y);
	hitBox.setScale(x, y);
}

void SpriteHitBox::setPosition(float x, float y)
{
	m_position = sf::Vector2f(x, y);
	sprite.setPosition(x, y);
	hitBox.setPosition(x, y);
	origin.setPosition(hitBox.getPosition());
}

void SpriteHitBox::setPosition(sf::Vector2f pos)
{
	setPosition(pos.x, pos.y);
}

sf::Vector2f SpriteHitBox::getScale()
{
	return m_scale;
}

sf::Vector2f SpriteHitBox::getPosition()
{
	return m_position;
}

sf::Vector2f SpriteHitBox::getActualSize()
{
	return m_actualSize;
}
