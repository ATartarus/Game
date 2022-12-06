#include "Entity.h"

Entity::Entity(sf::IntRect spriteFrame, sf::Vector2f hitBox, sf::Texture& texture, Origin_Pos position)
{
	this->sprite.setTextureRect(spriteFrame);

	switch (position)
	{
	case Origin_Pos::BOTTOM | Origin_Pos::CENTER:
		this->sprite.setOrigin(spriteFrame.width / 2.0f, (float)spriteFrame.height);
		this->hitBox.setOrigin(hitBox.x / 2.0f, hitBox.y);
		break;
	case Origin_Pos::CENTER:
		this->sprite.setOrigin(spriteFrame.width / 2.0f, spriteFrame.height / 2.0f);
		this->hitBox.setOrigin(hitBox.x / 2.0f, hitBox.y / 2.0f);
		break;
	}


	
	this->hitBox.setSize(hitBox);
	this->hitBox.setFillColor(sf::Color::Transparent);
	this->hitBox.setOutlineColor(sf::Color::Red);
	this->hitBox.setOutlineThickness(1.0f);

	this->showHitBox = false;
	this->showOrigin = false;

	m_position = this->hitBox.getPosition();
	m_scale = sf::Vector2f(1.0f, 1.0f);
	m_actualSize = hitBox;

	this->sprite.setTexture(texture);
}

void Entity::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	target.draw(sprite);
	if (showHitBox) target.draw(hitBox);
	if (showOrigin) {
		sf::CircleShape tmp(3.0f);
		tmp.setPosition(hitBox.getPosition());
		tmp.setOrigin(3.0f, 3.0f);
		tmp.setFillColor(sf::Color::Cyan);
		target.draw(tmp);
	}
}

void Entity::onWindowResize(sf::Vector2f scale)
{
	sf::Vector2f prodCoeff = sf::Vector2f(scale.x / fabs(m_scale.x), scale.y / fabs(m_scale.y));
	this->setScale((m_scale.x > 0) ? scale.x : -scale.x, scale.y);
	this->setPosition(this->getPosition().x * prodCoeff.x, this->getPosition().y * prodCoeff.y);
}

void Entity::move(float x, float y)
{
	m_position.x += x;
	m_position.y += y;
	sprite.move(x, y);
	hitBox.move(x, y);
}

void Entity::move(sf::Vector2f dir)
{
	move(dir.x, dir.y);
}

void Entity::setScale(float x, float y)
{
	m_actualSize = sf::Vector2f(m_actualSize.x / fabs(m_scale.x) * fabs(x), m_actualSize.y / fabs(m_scale.y) * fabs(y));
	m_scale = sf::Vector2f(x, y);
	sprite.setScale(x, y);
	hitBox.setScale(x, y);
}

void Entity::setScale(sf::Vector2f scale)
{
	setScale(scale.x, scale.y);
}

void Entity::setPosition(float x, float y)
{
	m_position = sf::Vector2f(x, y);
	sprite.setPosition(x, y);
	hitBox.setPosition(x, y);
}

void Entity::setPosition(sf::Vector2f pos)
{
	setPosition(pos.x, pos.y);
}

sf::Vector2f Entity::getScale()
{
	return m_scale;
}

sf::Vector2f Entity::getPosition()
{
	return m_position;
}

sf::Vector2f Entity::getActualBounds()
{
	return m_actualSize;
}

void Entity::flip(bool right)
{
	if (right) this->setScale(fabs(m_scale.x), m_scale.y);
	else this->setScale(-fabs(m_scale.x), m_scale.y);
}