#pragma once
#include "stdafx.h"

class Console : public sf::Drawable
{
private:
	std::string string;
	sf::RectangleShape background;
	sf::Font font;
	sf::Text text;

	unsigned fontSize = 14;
	sf::Vector2f scale = sf::Vector2f(1.0f, 1.0f);
public:
	Console();

	bool isEnabled = false;

	void setResolutionScale(sf::Vector2f scale);
	void setPosition(sf::Vector2f pos);
	void setOutput(std::string str);
	void clearOutput();
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
};

