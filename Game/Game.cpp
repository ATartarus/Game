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

const float Game::getDeltaTime() const
{
	return deltaTime;
}

void Game::setDeltaTime(float deltaTime)
{
	this->deltaTime = deltaTime;
}

void Game::update()
{
	this->updatePollEvent();

	this->updatePlayer();
}

void Game::updatePlayer()
{
	this->player->update(deltaTime);
}

void Game::render()
{
	this->window.clear();



	sf::RectangleShape* s = new sf::RectangleShape(sf::Vector2f(100,100));
	s->setPosition(0, 300);
	window.draw(*s);

	if (!s->getGlobalBounds().intersects(player->getSprite().getGlobalBounds())) {
		player->falling = true;
	}
	else player->falling = false;

	this->renderPlayer();
	this->window.display();
}

void Game::renderPlayer()
{
	this->player->render(this->window);
}
