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

	rect.setPosition(0.0f, 500.0f);
	rect.setSize(sf::Vector2f(800.0f, 100.0f));
	rect.setFillColor(sf::Color::White);
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

	this->updatePollEvent();

	this->updatePlayer();


	if (!rect.getGlobalBounds().intersects(player->getBody().hitBox.getGlobalBounds())) {
		player->falling = true;
	}
	else {
		if (player->getState() == FALLING) player->getBody().move(0.0f,rect.getPosition().y - player->getBody().hitBox.getPosition().y);
		player->falling = false;
	}
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

	window.draw(rect);
	window.draw(*map);
	this->renderPlayer();
	this->window.display();
}

void Game::renderPlayer()
{
	this->player->render(this->window);
}
