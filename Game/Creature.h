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

	float& deltaTime;
	float m_health;
	sf::Clock stagger;

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
	void initTexture(std::string png, std::string className);
	virtual void updateCollision() = 0;
	virtual void yCollisionCheck(sf::Vector2i& direction);
	void moveLeft();
	void moveRight();
	void moveJump();
public:
	Creature(sf::IntRect SpriteRect,
			 sf::Vector2f hitBox, 
			 std::string texture, 
			 std::vector<std::vector<Tile*>>& tiles, 
			 float& deltaTime,
			 std::string className = "Creature"
			);
	virtual ~Creature();

	Collider* collider;

	virtual void update() = 0;

	virtual void onDamageRecieve();
	virtual void onWindowResize(sf::Vector2f scale) override;
};

