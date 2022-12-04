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
	sf::Vector2f acceleration;
	float velocityMax;
	float deceleration;
	struct Jump {
		float height;
		bool allow;
		bool keyHold;
	} jump;

	//Initialization
	void initVariables();
	void initTexture();
public:
	Player();
	~Player();

	//Update
	void update(float deltaTime);
	void updateMovement(float deltaTime);
	void updateCollision(Entity& tile);
	std::string getFrameLog();

	void setResolutionScale(sf::Vector2f scale);
};

