#pragma once
#include "stdafx.h"
#include "Creature.h"

class Player : public Creature
{
private:
	void updateMovement();
	virtual void initVariables() override;
public:
	Player(std::vector<std::vector<Tile*>>& tiles, float& deltaTime);
	virtual ~Player();

	//Update
	virtual void update() override;

	void updateCollision();

	std::string getFrameLog();
};

