#include "Player.h"


//Initialization

Player::Player(sf::Texture& texture, std::vector<std::vector<Tile*>>& tiles, const float& deltaTime) : 
	    Creature(
			   sf::Vector2f(15.0f, 42.0f),
			   texture,
			   sf::IntRect(0, 0, 40, 50),
			   tiles,
			   deltaTime)
{
	initVariables();
	this->setScale(2.0f, 2.0f);
}

Player::~Player()
{
}

void Player::initVariables()
{
	staggerTime = 0.5f;
	velocityMax = 160.0f;
	acceleration.x = 10.0f;
	deceleration = acceleration.x * 3.0f;
}

//Update

void Player::update() 
{
	updateMovement();
	updateCollision();
	if (stagger.getElapsedTime().asSeconds() < staggerTime)
		animation->guardEffect();
	else sprite.setColor(sf::Color::White);
}

void Player::updateMovement()
{
	moveState = Move_State::IDLE;

	/*  <Velocity.x calculations>  */

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::A) || sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
	{
		moveLeft();
	}
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::D) || sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
	{
		moveRight();
	}


	if (fabs(velocity.x) > velocityMax)
		velocity.x = (velocity.x < 0) ? -velocityMax : velocityMax;


	if (moveState == Move_State::IDLE && velocity.x != 0) {
		velocity.x += (velocity.x < 0) ? deceleration : -deceleration;
		if (fabs(velocity.x) < deceleration) velocity.x = 0;
	}

	/*  </Velocity.x calculations>  */


	/*  <Velocity.y calculations>  */

	if (!sf::Keyboard::isKeyPressed(sf::Keyboard::W) && !sf::Keyboard::isKeyPressed(sf::Keyboard::Up) && (velocity.y >= 0.0f && velocity.y < 5.0f)) {
		jump.keyHold = false;
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::W) || sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
	{
		if (velocity.y < 0) jump.keyHold = true;
		moveJump();
	}
	
	if (velocity.y < 0) 		moveState = Move_State::JUMPING;
	else if (velocity.y > 0)	moveState = Move_State::FALLING;
	velocity.y += acceleration.y * deltaTime;


	/*  </Velocity.y calculations>  */

	this->move(velocity * deltaTime);

	animation->animate(moveState);
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


