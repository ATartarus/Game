#include "Game.h"


//Initialization

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

//Accessors

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

//Update

void Game::update()
{
	//std::cout << this->deltaTime << "\n";

	//int sum = 0;
	//for (int i = 0; i < 10000000; i++) {
	//	sum += i;
	//}

	this->updatePollEvent();

	this->updatePlayer();
}

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

void Game::updatePlayer()
{
	this->player->update(deltaTime);
}

//Render

void Game::render()
{
	this->window.clear();

	sf::RectangleShape rect;
	rect.setPosition(0, 500);
	rect.setSize(sf::Vector2f(800, 100));
	window.draw(rect);

	

	if (!rect.getGlobalBounds().intersects(player->getBody().hitBox.getGlobalBounds())) {
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
