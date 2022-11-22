#include "Player.h"


void Player::initVariables()
{
	playerState = IDLE;
	totalIdleTime = 0.0f;
	idleTime = 0.3f;
	totalRunTime = 0.0f;
	runTime = 0.1f;

	velocity.x = 200.0f;
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
	sprite.setTexture(texture);
	currentFrame = sf::IntRect(0, 0, 40, 50);
	sprite.setTextureRect(currentFrame);
	sprite.setScale(3.0f, 3.0f);
}

Player::Player()
{
	initVariables();
	initTexture();
	initSprite();
}

sf::Sprite Player::getSprite()
{
	return sprite;
}

void Player::update(float deltaTime)
{
	updateMovement(deltaTime);
	updateAnimation(deltaTime);
}

void Player::updateMovement(float deltaTime)
{
	playerState = IDLE;
	if (falling) {
		if (velocity.y < 0) playerState = JUMPING;
		else playerState = FALLING;
		velocity.y += 0.981f * deltaTime;
		sprite.move(0.0f, velocity.y);
	}
	else {
		velocity.y = 0;
	}


	if (sf::Keyboard::isKeyPressed(sf::Keyboard::A) || sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
	{

		sprite.move(-velocity.x * deltaTime, 0.0f);
		playerState = MOVING_LEFT;
		sprite.setScale(-3.0f, 3.0f);
		sprite.setOrigin(sprite.getGlobalBounds().width / 3.0f, 0.0f);
	}
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::D) || sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
	{
		sprite.move(velocity.x * deltaTime, 0.0f);
		playerState = MOVING_RIGHT;
		sprite.setScale(3.0f, 3.0f);
		sprite.setOrigin(0, 0);
	}

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::W) || sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
	{
		if (!falling) {
			velocity.y = -0.3;
			sprite.move(0.0f, velocity.y);
			playerState = JUMPING;
		}

	}
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::S) || sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
	{
		sprite.move(0.0f, velocity.y);
		playerState = FALLING;
	}
}

void Player::updateAnimation(float deltaTime)
{
	switch (playerState) {
	case IDLE:
		totalIdleTime += deltaTime;
		if (totalIdleTime >= idleTime)
		{
			totalIdleTime -= idleTime;
			currentFrame.top = 0;
			currentFrame.left += 40;
			if (currentFrame.left > 160) {
				currentFrame.left = 0;
			}

			sprite.setTextureRect(currentFrame);
		}
		totalRunTime = runTime;
		break;
	case MOVING_RIGHT:
		totalRunTime += deltaTime;
		if (totalRunTime >= runTime)
		{
			totalRunTime -= runTime;
			currentFrame.top = 50;
			currentFrame.left += 40;
			if (currentFrame.left > 360) {
				currentFrame.left = 0;
			}

			sprite.setTextureRect(currentFrame);
		}
		totalIdleTime = idleTime;
		break;
	case MOVING_LEFT:
		totalRunTime += deltaTime;
		if (totalRunTime >= runTime)
		{
			totalRunTime -= runTime;
			currentFrame.top = 50;
			currentFrame.left += 40;
			if (currentFrame.left > 360) {
				currentFrame.left = 0;
			}

			sprite.setTextureRect(currentFrame);
		}
		totalIdleTime = idleTime;
		break;
	
	case JUMPING:

		break;
	}
	
}

void Player::render(sf::RenderWindow& target)
{
	target.draw(sprite);
}

