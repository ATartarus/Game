#include "Player.h"


void Player::initVariables()
{
	animState = IDLE;
	totalIdleTime = 0.0f;
	idleTime = 0.3f;
	totalRunTime = 0.0f;
	runTime = 0.1f;
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
	currentFrame = sf::IntRect(0.0f, 0.0f, 40.0f, 50.0f);
	sprite.setTextureRect(currentFrame);
	sprite.setScale(3.0f, 3.0f);
}

Player::Player()
{
	initVariables();
	initTexture();
	initSprite();
}

void Player::update(float deltaTime)
{
	updateMovement(deltaTime);
	updateAnimation(deltaTime);
}

void Player::updateMovement(float deltaTime)
{
	animState = IDLE;
	float moveCoeff = 150.0f * deltaTime;

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::A) || sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
	{
		sprite.move(-moveCoeff, 0.0f);
		animState = MOVING_LEFT;
		sprite.setScale(-3.0f, 3.0f);
		sprite.setOrigin(sprite.getGlobalBounds().width / 3.0f, 0.0f);
	}
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::D) || sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
	{
		sprite.move(moveCoeff, 0.0f);
		animState = MOVING_RIGHT;
		sprite.setScale(3.0f, 3.0f);
		sprite.setOrigin(0, 0);
	}

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::W) || sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
	{
		sprite.move(0.0f, -moveCoeff);
		animState = JUMPING;
	}
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::S) || sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
	{
		sprite.move(0.0f, moveCoeff);
		animState = FALLING;
	}
}

void Player::updateAnimation(float deltaTime)
{
	switch (animState) {
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
	}
	
}

void Player::render(sf::RenderWindow& target)
{
	target.draw(sprite);
}

