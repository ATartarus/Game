#include "Player.h"


//Initialization

Player::Player() : Entity(sf::IntRect(0, 0, 40, 50), 
						  sf::Vector2f(15.0f, 42.0f),
						  texture,
						  Origin_Pos::BOTTOM | Origin_Pos::CENTER)
{
	initVariables();
	initTexture();
	this->setScale(2.0f, 2.0f);
	animation = new Animation(this->sprite);
}

Player::~Player()
{
	delete animation;
}

void Player::initVariables()
{
	playerState  = Player_State::IDLE;
	velocityMax  = 160.0f;
	acceleration = sf::Vector2f(10.0f, 981.0f);
	deceleration = acceleration.x * 3;
	velocity.x   = 0.0f;
	velocity.y   = 0.0f;
	jump.height  = 100.f;
	jump.allow   = true;
	jump.keyHold = false;
}

void Player::initTexture()
{
	if (!texture.loadFromFile("Texture\\player_sheet.png"))
	{
		std::cout << "PLAYER::Could not load texture" << "\n";
	}
}

//Update

void Player::update(float deltaTime)
{
	
	updateMovement(deltaTime);
}

void Player::updateMovement(float deltaTime)
{
	playerState = Player_State::IDLE;

	/*  <Velocity.x calculations>  */

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::A) || sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
	{
		velocity.x -= acceleration.x;
		playerState = Player_State::MOVING_LEFT;
		this->flip(false);
	}
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::D) || sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
	{
		velocity.x += acceleration.x;
		playerState = Player_State::MOVING_RIGHT;
		this->flip(true);
	}

	if (fabs(velocity.x) > velocityMax)
		velocity.x = (velocity.x < 0) ? -velocityMax : velocityMax;
	if (playerState == Player_State::IDLE && velocity.x != 0) {
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
		if (velocity.y == 0 && jump.allow) {
			velocity.y = -sqrtf(2.0f * acceleration.y * jump.height);
			jump.allow = false;
		}
	}
	
	if (velocity.y < 0) 		playerState = Player_State::JUMPING;
	else if (velocity.y > 0)	playerState = Player_State::FALLING;
	velocity.y += acceleration.y * deltaTime;


	/*  </Velocity.y calculations>  */

	this->move(velocity * deltaTime);

	animation->setDeltaTime(deltaTime);
	animation->animate(playerState);
}

void Player::updateCollision(Entity& tile)
{
	sf::Vector2i direction = Collider::Check(*this, tile);
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

std::string Player::getFrameLog()
{
	std::string str = "Position:\nX: " + std::to_string(m_position.x) + "\nY: " + std::to_string(m_position.y) + "\n";
	str += "Velocity:\nX: " + std::to_string(velocity.x) + "\nY: " + std::to_string(velocity.y) + "\n";
	return str;
}


void Player::setResolutionScale(sf::Vector2f scale)
{
	sf::Vector2f prodCoeff = sf::Vector2f(scale.x / fabs(m_scale.x), scale.y / fabs(m_scale.y));
	this->setScale((m_scale.x > 0) ? scale.x : -scale.x, scale.y);
	this->setPosition(this->getPosition().x * prodCoeff.x, this->getPosition().y * prodCoeff.y);
	velocity.y *= prodCoeff.y;
	velocityMax *= prodCoeff.x;
	acceleration *= prodCoeff.x;
	deceleration *= prodCoeff.x;
	jump.height *= prodCoeff.y;
}
