#include "WindowBase.h"


WindowBase::WindowBase(sf::RenderWindow& window, Switch_Flag& flag) :
	window(window), switchFlag(flag), e(sf::Event())
{
}

void WindowBase::onWindowResize()
{
	if (resolution.fullScreen.x / window.getSize().x == 1.0f &&
		resolution.fullScreen.y / window.getSize().y == 1.0f)
	{
		window.create(sf::VideoMode(resolution._default.x, resolution._default.y), "Game", sf::Style::Close);
	}
	else
	{
		window.create(sf::VideoMode(resolution.fullScreen.x, resolution.fullScreen.y), "Game", sf::Style::Fullscreen);
	}
}
