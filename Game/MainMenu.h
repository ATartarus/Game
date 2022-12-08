#pragma once
#include "WindowBase.h"

class MainMenu : public WindowBase
{
private:
	sf::Sprite background;
public:
	MainMenu(sf::RenderWindow& window, Switch_Flag& flag);
	~MainMenu();


	virtual void updateEvent() override;
	virtual void update() override;
	virtual void onWindowResize() override;
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
};

