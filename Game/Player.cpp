#include "Player.h"


void Player::initVariables()
{
	animState = IDLE;
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
	sprite.setTexture(this->texture);
	currentFrame = sf::IntRect(0, 0, 40, 50);
	sprite.setTextureRect(this->currentFrame);
	sprite.setScale(3, 3);
}

Player::Player()
{
	initTexture();
	initSprite();
}

void Player::update()
{
	updateMovement();
	updateAnimation();
}

void Player::updateMovement()
{
	animState = IDLE;

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::A) || sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
	{
		sprite.move(-1, 0);
		animState = MOVING_LEFT;
	}
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::D) || sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
	{
		sprite.move(1, 0);
		animState = MOVING_RIGHT;
	}

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::W) || sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
	{
		sprite.move(0, -1);
		animState = JUMPING;
	}
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::S) || sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
	{
		sprite.move(0, 1);
		animState = FALLING;
	}
}

void Player::updateAnimation()
{
	switch (animState) {
	case IDLE:
		if (animDelay.getElapsedTime().asSeconds() >= 0.3)
		{
			currentFrame.top = 0;
			currentFrame.left += 40;
			if (currentFrame.left > 160) {
				currentFrame.left = 0;
			}
			
			sprite.setTextureRect(currentFrame);
			animDelay.restart();
		}
		break;
	case MOVING_RIGHT:
		if (animDelay.getElapsedTime().asMilliseconds() >= 50)
		{
			currentFrame.top = 50;
			currentFrame.left += 40;

			if (currentFrame.left > 360) {
				currentFrame.left = 0;
			}

			
			sprite.setTextureRect(currentFrame);
			animDelay.restart();
		}
		break;
	}

	
}

void Player::render(sf::RenderWindow& target)
{
	target.draw(sprite);
}

