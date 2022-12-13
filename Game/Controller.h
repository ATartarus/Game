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
	sf::RenderWindow window;
	float deltaTime;
	float globalVolume;

	Scene* currentWindow;
	Switch_Flag switchFlag;

	void switchWindow();

public:
	Controller();
	~Controller();

	const sf::RenderWindow& getWindow() const;
	const float getDeltaTime() const;
	void setDeltaTime(const float deltaTime);


	void update();
	void render();
};

