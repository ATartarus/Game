#include "Console.h"

Console::Console()
{
	font.loadFromFile("Fonts\\arial.ttf");
	text.setFont(font);
	text.setCharacterSize(fontSize);
	text.setPosition(5.0f, 5.0f);
	text.setFillColor(sf::Color::Black);


	background.setSize(sf::Vector2f(150.0f, 200.0f));
	background.setFillColor(sf::Color::Color(85, 85, 85, 180));
}

void Console::onWindowResize(sf::Vector2f scale)
{
	background.setSize(sf::Vector2f(background.getSize().x * (scale.x / this->scale.x), background.getSize().y * (scale.y / this->scale.y)));
	text.setCharacterSize(fontSize *= (scale.x / this->scale.x));
	this->scale = scale;
}

void Console::setPosition(sf::Vector2f pos)
{
	background.setPosition(pos);
	text.setPosition(pos);
}

void Console::setOutput(std::string str)
{
	string += str;
	text.setString(string);
}

void Console::clearOutput()
{
	string.clear();
}

void Console::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	target.draw(background);
	target.draw(text);
}
