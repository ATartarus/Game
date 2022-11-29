#include "Player.h"


//Initialization

Player::Player()
{
	initVariables();
	initTexture();
	initSprite();
	animation = new Animation(body);
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
	allowJump = true;
	showHitBox = false;
	showOrigin = false;
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
	body = new SpriteHitBox(sf::IntRect(0, 0, 40, 50), sf::Vector2f(hitBoxWidth, hitBoxHeight), BOTTOM | CENTER);
	body->sprite.setTexture(texture);
	body->setScale(2.0f, 2.0f);
}

//Accessors

void Player::setPosition(float x, float y)
{
	body->setPosition(x, y);
}

void Player::setPosition(sf::Vector2f pos)
{
	setPosition(pos.x, pos.y);
}

sf::Vector2f Player::getPosition()
{
	return body->getPosition();
}


//Update

void Player::update(float deltaTime)
{
	updateMovement(deltaTime);
	body->showHitBox = showHitBox;
	body->showOrigin = showOrigin;
}

void Player::updateMovement(float deltaTime)
{
	playerState = Player_State::IDLE;

	/// <Velocity.x calculations>

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::A) || sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
	{
		velocity.x -= acceleration;
		playerState = Player_State::MOVING_LEFT;
		animation->flip(false);
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::D) || sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
	{
		velocity.x += acceleration;
		playerState = Player_State::MOVING_RIGHT;
		animation->flip(true);
	}

	if (fabs(velocity.x) > velocityMax)
		velocity.x = (velocity.x < 0) ? -velocityMax : velocityMax;
	if (playerState == Player_State::IDLE && velocity.x != 0) {
		velocity.x += (velocity.x < 0) ? deceleration : -deceleration;
		if (fabs(velocity.x) < deceleration) velocity.x = 0;
	}

	/// </Velocity.x calculations>


	/// <Velocity.y calculations>
	if (!sf::Keyboard::isKeyPressed(sf::Keyboard::W) && !sf::Keyboard::isKeyPressed(sf::Keyboard::Up) && velocity.y == 0) allowJump = true;
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::W) || sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
	{
		if (velocity.y == 0 && allowJump) {
			velocity.y = -sqrtf(2.0f * 981.0f * jumpHeight);
			allowJump = false;
		}
	}
	
	if (velocity.y < 0) 		playerState = JUMPING;
	else if (velocity.y > 0)	playerState = FALLING;
	velocity.y += 981.0f * deltaTime;


	/// </Velocity.y calculations>

	body->move(velocity * deltaTime);

	animation->setDeltaTime(deltaTime);
	animation->animate(playerState);
}

void Player::updateCollision(SpriteHitBox& tile)
{
	sf::Vector2i direction = Collider::Check(*(this->body), tile);
	if (direction.y != 0) {
		velocity.y = 0;
	}
}


//Render

void Player::render(sf::RenderWindow& target)
{
	target.draw(*body);
}

