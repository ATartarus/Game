#include "Player.h"


void Player::initVariables()
{
	playerState = IDLE;

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
	animation = new Animation(sprite, currentFrame);
}

sf::Sprite Player::getSprite()
{
	return sprite;
}

void Player::update(float deltaTime)
{
	updateMovement(deltaTime);
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
		if (!falling) playerState = MOVING_LEFT;
		sprite.setScale(-3.0f, 3.0f);
		sprite.setOrigin(sprite.getGlobalBounds().width / 3.0f, 0.0f);
	}
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::D) || sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
	{
		sprite.move(velocity.x * deltaTime, 0.0f);
		if (!falling) playerState = MOVING_RIGHT;
		sprite.setScale(3.0f, 3.0f);
		sprite.setOrigin(0, 0);
	}

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::W) || sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
	{
		if (!falling) {
			velocity.y = jumpVelocity;
			sprite.move(0.0f, velocity.y);
			playerState = JUMPING;
		}
	}

	animation->setDeltaTime(deltaTime);
	animation->animate(playerState);
}

void Player::render(sf::RenderWindow& target)
{
	target.draw(sprite);
}

