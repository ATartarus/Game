#include "Game.h"


//Cons/Dest

Game::Game() 
{
	this->window.create(sf::VideoMode(960, 640), "Game", sf::Style::Close);
	this->map = new Map("test.tmx");
	this->player = new Player();
	player->setPosition(200.0f, 200.0f);

	deltaTime = 0.0f;
	e = sf::Event();
}

Game::~Game()
{
	delete this->map;
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
	//for (int i = 0; i < 100000000; i++) {
	//	sum += i;
	//}

	updatePollEvent();

	updatePlayer();
}

void Game::updatePollEvent()
{
	while (this->window.pollEvent(this->e))
	{
		if (this->e.type == sf::Event::Closed)
		{
			this->window.close();
		}
		if (this->e.type == sf::Event::KeyPressed && e.key.code == sf::Keyboard::Tilde)
		{
			player->showHitBox = !player->showHitBox;
			player->showOrigin = !player->showOrigin;
			for (auto& tile : *(map->tileMap)) {
				tile->showHitBox = !tile->showHitBox;
				tile->showOrigin = !tile->showOrigin;
			}
		}
	}
}

void Game::updatePlayer()
{
	this->player->update(deltaTime);
	for (auto* tile : *(map->tileMap))
	{
		player->updateCollision(*tile);
	}
}

//Render

void Game::render()
{
	this->window.clear();

	window.draw(*map);
	this->renderPlayer();

	this->window.display();
}

void Game::renderPlayer()
{
	this->player->render(this->window);
}
