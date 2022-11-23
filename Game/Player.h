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
	sf::IntRect spritesheetFrame;
	Player_State playerState;
	Animation* animation;

	//Physics
	sf::Vector2f velocity;
	float veocityMax;
	float acceleration;
	float jumpVelocity;

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


	//Methods
	void update(float deltaTime);
	void updateMovement(float deltaTime);
	void render(sf::RenderWindow& target);
};

