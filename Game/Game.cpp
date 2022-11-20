#include "Game.h"

void Game::updatePollEvent()
{
	while (this->window.pollEvent(this->e))
	{
		if (this->e.type == sf::Event::Closed)
		{
			this->window.close();
		}
	}
}

void Game::initWindow()
{
	this->window.create(sf::VideoMode(800, 600), "Game", sf::Style::Close);
	this->window.setFramerateLimit(144);
}

void Game::initPlayer()
{
	this->player = new Player();
}

Game::Game()
{
	this->initWindow();
	this->initPlayer();
}

Game::~Game()
{
	delete this->player;
}

const sf::RenderWindow& Game::getWindow() const
{
	return this->window;
}

void Game::update()
{
	this->updatePollEvent();

	this->updatePlayer();
}

void Game::updatePlayer()
{
	this->player->update();
}

void Game::render()
{
	this->window.clear();

	this->renderPlayer();

	this->window.display();
}

void Game::renderPlayer()
{
	this->player->render(this->window);
}
