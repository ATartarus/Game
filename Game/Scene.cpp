#include "Scene.h"


Scene::Scene(sf::RenderWindow& window, Switch_Flag& flag) :
	window(window), switchFlag(flag), e(sf::Event())
{
	m_contentScale = sf::Vector2f(1, 1);
	textures.emplace("empty", sf::Texture());
}


void Scene::onWindowResize()
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

	m_contentScale = sf::Vector2f(window.getSize()) / resolution._default;
}
