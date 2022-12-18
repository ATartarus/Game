#pragma once
#include "Scene.h"
#include "Map.h"
#include "Player.h"
#include "Console.h"

class Game : public Scene
{
private:
	Player* player;
	Map* map;
	Console* console;
	const float& deltaTime;

	sf::Sprite gameOverBackground;
	sf::Sprite victoryBackground;
	bool end;

	std::vector<sf::Sprite> hearts;

	void changeMap(Map::Exit& exit);
	void focusView();
	void updateView();
	void updateConsole();
	void resizeContent(sf::Vector2f scale);

	void updateHearts();

	void loadTextures() override;
	void loadSounds() override;
public:
	Game(sf::RenderWindow& window, Switch_Flag& flag, float globalVolume, const float& deltaTime);
	~Game();


	void update() override;
	void updateEvent() override;
	void onWindowResize() override;
	void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
};

