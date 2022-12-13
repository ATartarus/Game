#pragma once
#include "Scene.h"
#include "Slider.h"

class MainMenu : public Scene
{
private:
	sf::Sprite background;
	std::vector<Slider> sliders;
	bool isInOptions;
	float& globalVolume;

	void loadTextures() override;
	void loadSounds() override;
	void initMainMenu();
	void initOptionsMenu();
	void resizeContent();
public:
	MainMenu(sf::RenderWindow& window, Switch_Flag& flag, float& globalVolume);
	~MainMenu();


	void updateEvent() override;
	void update() override;
	void onWindowResize() override;
	void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
};

