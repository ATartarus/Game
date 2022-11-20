#pragma once
#include "stdafx.h"
#include "Player.h"

class Game
{
private:
	//Window

	sf::RenderWindow window;
	sf::Event e;

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

	//Methods

	void update();
	void updatePlayer();
	void render();
	void renderPlayer();
};

