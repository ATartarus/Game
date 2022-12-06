#include "Creature.h"

void Creature::initVariables()
{
	m_health = 10.0f;


	moveState = Move_State::IDLE;
	velocityMax = 100.0f;
	acceleration = sf::Vector2f(5.0f, 981.0f);
	deceleration = acceleration.x * 3;
	jump.height = 100.f;
	jump.allow = true;
	jump.keyHold = false;
}

void Creature::initTexture(std::string png, std::string className = "Creature")
{
	std::string PATH = "Texture\\" + png;
	if (!texture.loadFromFile(PATH))
	{
		std::cout << className << "::Could not load texture" << "\n";
	}
}

Creature::Creature(sf::IntRect SpriteRect, 
				   sf::Vector2f hitBox, 
				   std::string textureFile, 
				   std::vector<std::vector<Tile*>>& tiles, 
				   float& deltaTime,
				   std::string className
				  ) :
			Entity(SpriteRect, hitBox, texture, Origin_Pos::BOTTOM | Origin_Pos::CENTER), 
			deltaTime(deltaTime)
{
	initVariables();
	initTexture(textureFile, className);
	animation = new Animation(this->sprite, deltaTime);
	collider = new Collider(*this, tiles);
}

Creature::~Creature()
{
	delete animation;
	delete collider;
}

void Creature::moveLeft()
{
	velocity.x -= acceleration.x;
	moveState = Move_State::MOVING_LEFT;
	this->flip(false);
}

void Creature::moveRight()
{
	velocity.x += acceleration.x;
	moveState = Move_State::MOVING_RIGHT;
	this->flip(true);
}

void Creature::moveJump()
{
	if (velocity.y == 0 && jump.allow) {
		velocity.y = -sqrtf(2.0f * acceleration.y * jump.height);
		jump.allow = false;
	}
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

void Creature::onDamageRecieve()
{
	if (stagger.getElapsedTime().asSeconds() >= 0.5f)
	{
		stagger.restart();
		m_health -= 1.0f;
	}
}

void Creature::onWindowResize(sf::Vector2f scale)
{
	sf::Vector2f prodCoeff = sf::Vector2f(scale.x / fabs(m_scale.x), scale.y / fabs(m_scale.y));
	Entity::onWindowResize(scale);
	velocity.y *= prodCoeff.y;
	velocityMax *= prodCoeff.x;
	acceleration *= prodCoeff.x;
	deceleration *= prodCoeff.x;
	jump.height *= prodCoeff.y;
	collider->mapChange();
}
