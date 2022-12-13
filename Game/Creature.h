#pragma once
#include "Tile.h"
#include "Animation.h"
#include "Collider.h"


class Creature : public Entity
{
protected:
	sf::Texture texture;
	Move_State moveState;
	Animation* animation;

	const float& deltaTime;
	float m_health;
	sf::Clock stagger;
	float staggerTime;

	sf::Vector2f velocity;
	sf::Vector2f acceleration;
	float velocityMax;
	float deceleration;
	struct Jump {
		float height;
		bool allow;
		bool keyHold;
	} jump;

	virtual void initVariables();
	virtual void updateCollision() = 0;
	virtual void yCollisionCheck(sf::Vector2i& direction);
	void moveLeft();
	void moveRight();
	void moveJump();
public:
	Creature(sf::Vector2f hitBox,
			 sf::Texture* texture,
		 	 sf::IntRect textureRect,
			 std::vector<std::vector<Tile*>>& tiles, 
			 const float& deltaTime
			);
	~Creature();

	Collider* collider;

	bool isAlive;

	virtual void update() = 0;

	void onDamageRecieve();
	void onWindowResize(sf::Vector2f scale) override;
};

