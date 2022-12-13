#pragma once
#include "Entity.h"
#include <functional>

class Slider : public Entity
{
private:
	sf::RenderWindow& window;
	Entity thumb;
	sf::Sound sound1;
	sf::Sound sound2;
	sf::Sound sound3;
	std::function<void()> dragHandler;
	
	double tick;
	float& bindedValue;
	bool leftPressed;
public:
	Slider(sf::RenderWindow& window, sf::Vector2f lineSize, sf::Vector2f thumbSize, float& value, float maxValue,
		   sf::Texture* lineTexture = nullptr, sf::Texture* thumbTexture = nullptr);

	void setPosition(float x, float y);
	void setPosition(sf::Vector2f pos);
	void setScale(float x, float y) override;
	void setScale(sf::Vector2f scale) override;
	void setDragHandler(std::function<void()> handler);
	void setSoundBuffer(const sf::SoundBuffer& buffer);


	void update();


	void onWindowResize(sf::Vector2f scale) override;
	void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
};

