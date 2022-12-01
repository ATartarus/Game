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
		if (this->e.type == sf::Event::KeyPressed && e.key.code == sf::Keyboard::F1)
		{
			map->hitBoxesVisible = !map->hitBoxesVisible;
			player->showHitBox = !player->showHitBox;
		}
		if (this->e.type == sf::Event::KeyPressed && e.key.code == sf::Keyboard::F2)
		{
			map->originsVisible = !map->originsVisible;
			player->showOrigin = !player->showOrigin;
		}
	}
}

void Game::updatePlayer()
{
	this->player->update(deltaTime);
	for (auto* tile : *(map->foregroundTiles))
	{
		player->updateCollision(*tile);
	}

	if ((player->getPosition().x >= map->exit->x1 && player->getPosition().x <= map->exit->x2) && 
		(player->getPosition().y >= map->exit->y1 && player->getPosition().y <= map->exit->y2))
	{
		std::string nextMap = map->exit->nextMap;
		delete map;
		map = new Map(nextMap.c_str());
		player->setPosition((player->getPosition().x > map->exit->x1) ? 
							 map->exit->x2 + 5 :
							 map->exit->x1 - 5,
							 player->getPosition().y);
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
