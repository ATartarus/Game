#pragma once
#include "stdafx.h"
#include "Button.h"

class Scene : public sf::Drawable
{
protected:

	sf::RenderWindow& window;
	sf::Event e;
	struct Resolution {
		const sf::Vector2f _default = sf::Vector2f(960.0f, 540.0f);
		const sf::Vector2f fullScreen = sf::Vector2f(1920.0f, 1080.0f);
	} resolution;

protected:

	std::map<std::string, sf::Texture> textures;
	std::map<std::string, sf::SoundBuffer> sounds;
	std::vector<Button> buttons;

protected:

	Switch_Flag& switchFlag;
	sf::Vector2f m_contentScale;
	sf::Font font;


	virtual void loadTextures() = 0;
	virtual void loadSounds() = 0;
public:
	Scene(sf::RenderWindow& window, Switch_Flag& flag);
	virtual ~Scene() = 0 {};


	virtual void update() = 0;
	virtual void updateEvent() = 0;
	virtual void onWindowResize() = 0;
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override = 0;
};

