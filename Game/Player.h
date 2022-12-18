#pragma once
#include "stdafx.h"
#include "Creature.h"

class Player : public Creature
{
private:
	void updateMovement();
	void initVariables() override;

	sf::Clock jumpDelay;

public:
	Player(sf::Texture* texture, std::vector<std::vector<Tile*>>& tiles, const float& deltaTime);
	~Player();

	void update() override;

	void updateCollision();

	std::string getFrameLog();
};

