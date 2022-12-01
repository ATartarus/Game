#pragma once
#include "stdafx.h"
#include "Map.h"
#include "Player.h"

class Game
{
private:
	//Window

	sf::RenderWindow window;
	sf::Event e;
	float deltaTime;

	//Map
	Map* map;

	//Player
	Player* player;

	void updatePollEvent();
public:
	Game();
	~Game();

	//Accessors

	const sf::RenderWindow& getWindow() const;
	const float getDeltaTime() const;
	void setDeltaTime(float deltaTime);

	//Update

	void update();
	void updatePlayer();
	//Render

	void render();
	void renderPlayer();
};

