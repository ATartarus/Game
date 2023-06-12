#include "Player.h"


//Initialization

Player::Player(sf::Texture* texture, Map& map, const float& deltaTime) : 
	    Creature(
			   sf::Vector2f(15.0f, 42.0f),
			   texture,
			   sf::IntRect(0, 0, 40, 50),
			   *map.foregroundTiles,
			   deltaTime)
{
	initVariables();
	collider = new Collider(*this, map);
	animation = new Animation(this->sprite, staggerTime, deltaTime);
}

Player::~Player()
{
}

void Player::initVariables()
{
	Creature::initVariables();
	staggerTime = 2.0f;
	velocityMax = 160.0f;
	acceleration.x = 10.0f;
	deceleration = sf::Vector2f(acceleration.x * 3, 10.f);
	staggered = false;
}

//Update

void Player::update() 
{
	updateMovement();
	updateCollision();


	if (stagger.getElapsedTime().asSeconds() < staggerTime && m_health != 10)
	{
		if (stagger.getElapsedTime().asSeconds() > staggerTime / 2.0f) staggered = false;
		animation->guardEffect();
	}
	else sprite.setColor(sf::Color::White);
	
	
	animation->animate(state);
}

void Player::updateMovement()
{
    state = Creature_State::IDLE;

	/*  <Velocity.x calculations>  */

	if (!staggered)
	{
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::A) || sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
		{
			moveLeft();
			if (stepDelay.getElapsedTime().asSeconds() > 0.55 && velocity.y == 0) { step.play(); stepDelay.restart(); }
		}
		else if (sf::Keyboard::isKeyPressed(sf::Keyboard::D) || sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
		{
			moveRight();
			if (stepDelay.getElapsedTime().asSeconds() > 0.55 && velocity.y == 0) { step.play(); stepDelay.restart(); }
		}
	}


	if (fabs(velocity.x) > velocityMax)
		velocity.x = (velocity.x < 0) ? -velocityMax : velocityMax;


	if (state == Creature_State::IDLE && velocity.x != 0) {
		velocity.x += (velocity.x < 0) ? deceleration.x : -deceleration.x;
		if (fabs(velocity.x) < deceleration.x) velocity.x = 0;
	}


	/*  </Velocity.x calculations>  */


	/*  <Velocity.y calculations>  */

	if (!sf::Keyboard::isKeyPressed(sf::Keyboard::W) && !sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) {
		jump.keyHold = false;
	}
	if ((sf::Keyboard::isKeyPressed(sf::Keyboard::W) || sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) && !staggered)
	{
		if (velocity.y < 0 && jumpDelay.getElapsedTime().asSeconds() > 0.1 && jump.keyHold)
		{
			velocity.y += velocity.y * 2 * deltaTime;
		}		
		if (velocity.y == 0 && jump.allow) {
			velocity.y = -sqrtf(2.0f * g * jump.height);
			jump.allow = false;
			jump.keyHold = true;

			jumpDelay.restart();
		}
	}


	if (velocity.y < 0) 		state = Creature_State::JUMPING;
	else if (velocity.y > 0)	state = Creature_State::FALLING;
	velocity.y += g * deltaTime;


	/*  </Velocity.y calculations>  */

	this->move(velocity * deltaTime);
}



void Player::updateCollision()
{
	sf::Vector2i direction = collider->Check();
	yCollisionCheck(direction);
}




std::string Player::getFrameLog()
{
	std::string str = "Position:\nX: " + std::to_string(m_position.x) + "\nY: " + std::to_string(m_position.y) + "\n";
	str += "Velocity:\nX: " + std::to_string(velocity.x) + "\nY: " + std::to_string(velocity.y) + "\n";
	str += "Health: " + std::to_string(m_health) + "\n";
	return str;
}


