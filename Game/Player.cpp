#include "Player.h"


//Initialization

Player::Player()
{
	initVariables();
	initTexture();
	initSprite();
	animation = new Animation(body->sprite, *spritesheetFrame);
}

Player::~Player()
{
	delete spritesheetFrame;
	delete body;
	delete animation;
}

void Player::initVariables()
{
	playerState = IDLE;

	velocityMax = 360.0f;
	acceleration = 10.0f;
	deceleration = acceleration / 5;
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
	spritesheetFrame = new sf::IntRect(0, 0, 40, 50);
	body = new SpriteHitBox(*spritesheetFrame, sf::Vector2f(hitBoxWidth, hitBoxHeight));
	body->sprite.setTexture(texture);
	body->setScale(2.0f, 2.0f);
}

//Accessors

SpriteHitBox& Player::getBody()
{
	return *body;
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
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::D) || sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
	{
		velocity.x += acceleration;
		playerState = MOVING_RIGHT;
	}

	if (fabs(velocity.x) > velocityMax)
		velocity.x = (velocity.x < 0) ? -velocityMax : velocityMax;
	if (playerState == IDLE && velocity.x != 0) {
		velocity.x += (velocity.x < 0) ? deceleration : -deceleration;
		if (fabs(velocity.x) < 1) velocity.x = 0;
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
	target.draw(body->sprite);
	target.draw(body->hitBox);
}

