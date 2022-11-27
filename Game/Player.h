#pragma once
#include "stdafx.h"
#include "Animation.h"
#include "SpriteHitBox.h"

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



	//Initialization
	void initVariables();
	void initTexture();
	void initSprite();
public:
	Player();
	~Player();
	bool falling = true;

	//Accessors
	SpriteHitBox& getBody();
	Player_State getState();

	//Update
	void update(float deltaTime);
	void updateMovement(float deltaTime);
	void setPosition(float x, float y);
	void setPosition(sf::Vector2f pos);

	//Render
	void render(sf::RenderWindow& target);
};

