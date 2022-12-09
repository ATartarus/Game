#pragma once
#include "stdafx.h"
#include "Map.h"
#include "Player.h"
#include "Console.h"
#include "Game.h"
#include "MainMenu.h"

class Controller
{
private:
	//Window
	sf::RenderWindow window;
	float deltaTime;

	Scene* currentWindow;
	Switch_Flag switchFlag;

	void switchWindow();

public:
	Controller();
	~Controller();

	//Accessors

	const sf::RenderWindow& getWindow() const;
	const float getDeltaTime() const;
	void setDeltaTime(const float deltaTime);


	void update();

	void render();
};

