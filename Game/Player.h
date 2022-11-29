#pragma once
#include "stdafx.h"
#include "Animation.h"
#include "SpriteHitBox.h"
#include "Collider.h"

class Player
{
private:
	//Model
	sf::Texture texture;
	SpriteHitBox* body;
	const float hitBoxHeight = 42.0f;
	const float hitBoxWidth = 25.0f;

	//Animation
	Player_State playerState;
	Animation* animation;

	//Physics
	sf::Vector2f velocity;
	float velocityMax;
	float acceleration;
	float deceleration;
	float jumpHeight;
	bool allowJump;

	//Initialization
	void initVariables();
	void initTexture();
	void initSprite();
public:
	Player();
	~Player();

	bool showHitBox;
	bool showOrigin;

	//Accessors
	void setPosition(float x, float y);
	void setPosition(sf::Vector2f pos);
	sf::Vector2f getPosition();

	//Update
	void update(float deltaTime);
	void updateMovement(float deltaTime);
	void updateCollision(SpriteHitBox& tile);

	//Render
	void render(sf::RenderWindow& target);
};

