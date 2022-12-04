#pragma once
#include "stdafx.h"
#include "Map.h"
#include "Player.h"
#include "Console.h"

class Game
{
private:
	//Window
	struct Resolution {
		const sf::Vector2f _default = sf::Vector2f(960.0f, 540.0f);
		const sf::Vector2f fullScreen = sf::Vector2f(1920.0f, 1080.0f);
	} resolution;
	sf::RenderWindow window;
	sf::Event e;
	float deltaTime;


	Console* console;
	Map* map;
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
	void updateView();
	void updateConsole();

	//Util

	void changeMap();
	void focusView();

	//Render

	void render();

	void onWindowResize();
};

