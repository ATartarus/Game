#pragma once
#include "stdafx.h"
#include "Player.h"

class Game
{
private:
	//Window

	sf::RenderWindow window;
	sf::Event e;
	float deltaTime;

	//Player
	Player* player;

	void updatePollEvent();
	void initWindow();
	void initPlayer();
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

