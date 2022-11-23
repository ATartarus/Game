#include "Player.h"


void Player::initVariables()
{
	playerState = IDLE;

	veocityMax = 100.0f;
	acceleration = 10.0f;
	jumpVelocity = -0.35f;
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
	spritesheetFrame = sf::IntRect(0, 0, 40, 50);
	body = new SpriteHitBox(spritesheetFrame, sf::Vector2f(hitBoxWidth, hitBoxHeight));
	body->sprite.setTexture(texture);
	body->setScale(2.0f, 2.0f);
}

Player::Player()
{
	initVariables();
	initTexture();
	initSprite();
	animation = new Animation(body->sprite, spritesheetFrame);
}

Player::~Player()
{
	delete body;
	delete animation;
}

SpriteHitBox& Player::getBody()
{
	return *body;
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
		body->move(0.0f, velocity.y);
	}
	else {
		velocity.y = 0;
	}


	if (sf::Keyboard::isKeyPressed(sf::Keyboard::A) || sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
	{

		body->move(-velocity.x * deltaTime, 0.0f);
		if (!falling) playerState = MOVING_LEFT;
		body->setScale(-2.0f, 2.0f);
		body->sprite.setOrigin(body->sprite.getGlobalBounds().width / 2.0f, 0.0f);
	}
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::D) || sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
	{
		body->move(velocity.x * deltaTime, 0.0f);
		if (!falling) playerState = MOVING_RIGHT;
		body->setScale(2.0f, 2.0f);
		body->sprite.setOrigin(0, 0);
	}

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::W) || sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
	{
		if (!falling) {
			velocity.y = jumpVelocity;
			body->move(0.0f, velocity.y);
			playerState = JUMPING;
		}
	}

	animation->setDeltaTime(deltaTime);
	animation->animate(playerState);
}

void Player::render(sf::RenderWindow& target)
{
	target.draw(body->sprite);
	target.draw(body->hitBox);
}

