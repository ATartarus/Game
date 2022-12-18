#include "Creature.h"


Creature::Creature(sf::Vector2f hitBox,
				   sf::Texture* texture,
				   sf::IntRect textureRect,
				   std::vector<std::vector<Tile*>>& tiles,
				   const float& deltaTime
				  ) :
			Entity(hitBox, texture, textureRect, Origin_Pos::BOTTOM | Origin_Pos::CENTER), 
			deltaTime(deltaTime)
{
	initVariables();
}

Creature::~Creature()
{
	delete animation;
	delete collider;
}


void Creature::initVariables()
{
	m_health = 5;
	staggerTime = 2.0f;

	isAlive = true;
	state = Creature_State::IDLE;
	velocityMax = 100.0f;
	g = 981.0f;
	jump.height = 80.f;
	acceleration = sf::Vector2f(5.0f, g);
	deceleration = sf::Vector2f(acceleration.x * 3, 10.0f);
	jump.allow = true;
	jump.keyHold = false;
	staggered = false;

}


void Creature::moveLeft()
{
	if (velocity.y != 0) velocity.x -= acceleration.x / deceleration.y;
	else velocity.x -= acceleration.x;
	state = Creature_State::MOVING_LEFT;
	this->flip(false);
}

void Creature::moveRight()
{
	if (velocity.y != 0) velocity.x += acceleration.x / deceleration.y;
	else velocity.x += acceleration.x;
	state = Creature_State::MOVING_RIGHT;
	this->flip(true);
}


void Creature::yCollisionCheck(sf::Vector2i& direction)
{
	if (direction.y != 0) {
		if (direction.y == 1 && velocity.y > 0) {		//Hitting tile from top
			velocity.y = 0;
			if (!jump.keyHold) jump.allow = true;
		}
		else if (direction.y == -1 && velocity.y < 0) {	//Hitting tile from bottom
			velocity.y = 0;
		}
	}
}

unsigned int Creature::getHP()
{
	return m_health;
}

void Creature::onDamageRecieve()
{
	
	if (stagger.getElapsedTime().asSeconds() >= staggerTime)
	{
		stagger.restart();
		m_health -= 1;
		injury.play();

		if (m_health <= 0) isAlive = false;
	}
	else if (stagger.getElapsedTime().asSeconds() < staggerTime / 2.0f) {
		state |= Creature_State::INJURED;
		staggered = true;
	}
	else staggered = false;
}

void Creature::setSoundBuffer(sf::SoundBuffer& buffer)
{
	injury.setBuffer(buffer);
}

void Creature::setVolume(float vol)
{
	injury.setVolume(vol);
}

void Creature::onWindowResize(sf::Vector2f scale)
{
	sf::Vector2f prodCoeff = sf::Vector2f(scale.x / fabs(m_scale.x), scale.y / fabs(m_scale.y));
	Entity::onWindowResize(scale);
	velocity.y *= prodCoeff.y;
	velocityMax *= prodCoeff.x;
	acceleration *= prodCoeff.x;
	deceleration.x *= prodCoeff.x;
	deceleration.y *= fabs(m_scale.x) / scale.x;
	g *= prodCoeff.y;
	jump.height *= prodCoeff.y;
	collider->onMapScaleChange();
}