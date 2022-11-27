#include "Player.h"


//Initialization

Player::Player()
{
	initVariables();
	initTexture();
	initSprite();
	animation = new Animation(*body);
}

Player::~Player()
{
	delete body;
	delete animation;
}

void Player::initVariables()
{
	playerState = IDLE;

	velocityMax = 160.0f;
	acceleration = 10.0f;
	deceleration = acceleration * 3;
	jumpHeight = 100.0f;
	velocity.x = 0.0f;
	velocity.y = 0.0f;
}

void Player::initTexture()
{
	if (!texture.loadFromFile("Texture/player_sheet.png"))
	{
		std::cout << "PLAYER::Could not load texture" << "\n";
	}
}

void Player::initSprite()
{
	body = new SpriteHitBox(sf::IntRect(0, 0, 40, 50), sf::Vector2f(hitBoxWidth, hitBoxHeight));
	body->sprite.setTexture(texture);
	body->setScale(2.0f, 2.0f);
	body->showHitBox = true;
}

//Accessors

SpriteHitBox& Player::getBody()
{
	return *body;
}

void Player::setPosition(float x, float y)
{
	body->setPosition(x, y);
}

void Player::setPosition(sf::Vector2f pos)
{
	setPosition(pos.x, pos.y);
}

Player_State Player::getState()
{
	return playerState;
}

//Update

void Player::update(float deltaTime)
{
	updateMovement(deltaTime);
}

void Player::updateMovement(float deltaTime)
{
	playerState = IDLE;

	/// <Velocity.x calculations>

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::A) || sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
	{
		velocity.x -= acceleration;
		playerState = MOVING_LEFT;
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::D) || sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
	{
		velocity.x += acceleration;
		playerState = MOVING_RIGHT;
	}

	if (fabs(velocity.x) > velocityMax)
		velocity.x = (velocity.x < 0) ? -velocityMax : velocityMax;
	if (playerState == IDLE && velocity.x != 0) {
		velocity.x += (velocity.x < 0) ? deceleration : -deceleration;
		if (fabs(velocity.x) < deceleration) velocity.x = 0;
	}

	/// </Velocity.x calculations>


	/// <Velocity.y calculations>

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::W) || sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
	{
		if (!falling) {
			velocity.y = -sqrtf(2.0f * 981.0f * jumpHeight);
			falling = true;
		}
	}

	if (falling) {
		if (velocity.y < 0) playerState = JUMPING;
		else				playerState = FALLING;
		velocity.y += 981.0f * deltaTime;
	}
	else {
		velocity.y = 0;
	}

	/// </Velocity.y calculations>

	body->move(velocity * deltaTime);

	animation->setDeltaTime(deltaTime);
	animation->animate(playerState);
}

//Render

void Player::render(sf::RenderWindow& target)
{
	target.draw(*body);
}

