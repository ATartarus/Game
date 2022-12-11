#pragma once
#include "Scene.h"


class MainMenu : public Scene
{
private:
	sf::Sprite background;

	void loadTextures() override;
	void loadSounds() override;
	void initButtons();
public:
	MainMenu(sf::RenderWindow& window, Switch_Flag& flag);
	~MainMenu();


	void updateEvent() override;
	void update() override;
	void onWindowResize() override;
	void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
};

