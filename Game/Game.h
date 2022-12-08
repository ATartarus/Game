#pragma once
#include "WindowBase.h"
#include "Map.h"
#include "Player.h"
#include "Console.h"

class Game : public WindowBase
{
private:
	Player* player;
	Map* map;
	Console* console;
	const float& deltaTime;

	sf::Sprite gameOverBackground;

	void changeMap();
	void focusView();
	void updateView();
	void updateConsole();
public:
	Game(sf::RenderWindow& window, Switch_Flag& flag, const float& deltaTime);
	~Game();


	virtual void update() override;
	virtual void updateEvent() override;
	virtual void onWindowResize() override;
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
};

