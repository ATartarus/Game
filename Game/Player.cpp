#include "Player.h"


//Initialization

Player::Player() : Entity(sf::IntRect(0, 0, 40, 50), 
						  sf::Vector2f(15.0f, 42.0f),
						  texture,
						  Origin_Pos::BOTTOM | Origin_Pos::CENTER)
{
	initVariables();
	initTexture();
	initSprite();
	animation = new Animation(this->sprite);
}

Player::~Player()
{
	delete animation;
}

void Player::initVariables()
{
	playerState = Player_State::IDLE;
	velocityMax = 160.0f;
	acceleration = 10.0f;
	deceleration = acceleration * 3;
	jumpHeight = 100.0f;
	velocity.x = 0.0f;
	velocity.y = 0.0f;
	allowJump = true;
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
	this->setScale(2.0f, 2.0f);
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
		velocity.x -= acceleration;
		playerState = Player_State::MOVING_LEFT;
		this->flip(false);
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::D) || sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
	{
		velocity.x += acceleration;
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
 
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::W) || sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
	{
		if (velocity.y == 0 && allowJump) {
			velocity.y = -sqrtf(2.0f * 981.0f * jumpHeight);
			allowJump = false;
		}
	}
	
	if (velocity.y < 0) 		playerState = Player_State::JUMPING;
	else if (velocity.y > 0)	playerState = Player_State::FALLING;
	velocity.y += 981.0f * deltaTime;


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
			allowJump = true;
		}
		else if (direction.y == -1 && velocity.y < 0) {	//Hitting tile from bottom
			velocity.y = 0;
		}
	}
}


//Render

void Player::render(sf::RenderWindow& target)
{
	target.draw(*this);
}