#pragma once
#include "stdafx.h"

class WindowBase : public sf::Drawable
{
protected:
	sf::RenderWindow& window;
	sf::Event e;
	Switch_Flag& switchFlag;
	sf::Font font;

	std::map<std::string, sf::Texture> textures;

	struct Resolution {
		const sf::Vector2f _default = sf::Vector2f(960.0f, 540.0f);
		const sf::Vector2f fullScreen = sf::Vector2f(1920.0f, 1080.0f);
	} resolution;

public:
	WindowBase(sf::RenderWindow& window, Switch_Flag& flag);
	virtual ~WindowBase() = 0 {};


	virtual void update() = 0;
	virtual void updateEvent() = 0;
	virtual void onWindowResize() = 0;
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override = 0;
};

