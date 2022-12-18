#pragma once
#include "Tile.h"
#include "Animation.h"
#include "Collider.h"


class Creature : public Entity
{
protected:
	sf::Texture texture;
	Creature_State state;
	Animation* animation;

	const float& deltaTime;
	unsigned int m_health;
	sf::Clock stagger;
	float staggerTime;
	bool staggered;

	sf::Sound injury;

	sf::Vector2f velocity;
	sf::Vector2f acceleration;
	sf::Vector2f deceleration;
	float g;
	float velocityMax;
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

	unsigned int getHP();
	void setSoundBuffer(sf::SoundBuffer& buffer);
	void setVolume(float vol);

	virtual void update() = 0;

	void onDamageRecieve();
	void onWindowResize(sf::Vector2f scale) override;
};

