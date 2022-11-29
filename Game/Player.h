#pragma once
#include "stdafx.h"
#include "Animation.h"
#include "Entity.h"
#include "Collider.h"

class Player : public Entity
{
private:
	//Model
	sf::Texture texture;

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

	//Update
	void update(float deltaTime);
	void updateMovement(float deltaTime);
	void updateCollision(Entity& tile);

	//Render
	void render(sf::RenderWindow& target);
};

