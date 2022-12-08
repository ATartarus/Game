#include "MainMenu.h"

MainMenu::MainMenu(sf::RenderWindow& window, Switch_Flag& flag) : WindowBase(window, flag)
{
	/* Load texture and other resources*/
	sf::Texture tmp;
	tmp.loadFromFile("Texture\\MainMenu.png");
	textures.emplace("background", tmp);


	background.setTexture(textures["background"], true);
	background.setScale(window.getSize().x / background.getGlobalBounds().width,
						window.getSize().y / background.getGlobalBounds().height);
}

MainMenu::~MainMenu()
{
}

void MainMenu::updateEvent()
{
	while (window.pollEvent(e))
	{
		if (e.type == sf::Event::Closed)
		{
			window.close();
		}
		if (e.type == sf::Event::KeyPressed && e.key.code == sf::Keyboard::F3)
		{
			onWindowResize();
		}
		if (e.type == sf::Event::KeyPressed && e.key.code == sf::Keyboard::Enter)
		{
			switchFlag = Switch_Flag::GAME;
		}
	}
}

void MainMenu::update()
{
	updateEvent();
}

void MainMenu::onWindowResize()
{
	WindowBase::onWindowResize();
	background.setScale(window.getSize().x / background.getGlobalBounds().width,
						window.getSize().y / background.getGlobalBounds().height);
}

void MainMenu::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	target.draw(background);
}
