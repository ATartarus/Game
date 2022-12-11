#pragma once
#include "Entity.h"
#include <functional>

class MainMenu;

class Button : public Entity
{
private:
	sf::RenderTarget& window;
	sf::Text text;
	sf::Sound sound;
private:
	std::function<void()> handler;
	unsigned int charSize;
	bool leftPressed;
	bool hovered;
public:
	Button(sf::RenderWindow& window, sf::Font& font, sf::Vector2i size, sf::Texture& texture);


	void setCharacterSize(unsigned int size);
	void setFillColor(sf::Color& color);
	void setString(std::string str);
	void setPosition(float x, float y) override;
	void setPosition(sf::Vector2f pos) override;
	void setScale(float x, float y) override;
	void setScale(sf::Vector2f pos) override;
	void setPressHandler(std::function<void()> handler);
	void setSoundBuffer(const sf::SoundBuffer& buffer);


	void update();

	void onWindowResize(sf::Vector2f scale) override;
	void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
};

