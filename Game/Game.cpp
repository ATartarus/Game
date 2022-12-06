#include "Game.h"


//Cons/Dest

Game::Game() 
{
	window.create(sf::VideoMode(960, 540), "Game", sf::Style::Close);
	map = new Map("test.tmx");
	player = new Player(*map->foregroundTiles, deltaTime);
	player->setPosition(200.0f, 200.0f);
	console = new Console();


	deltaTime = 0.0f;
	e = sf::Event();
}

Game::~Game()
{
	delete map;
	delete player;
	delete console;
}

//Accessors

const sf::RenderWindow& Game::getWindow() const
{
	return window;
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
	console->clearOutput();

	updatePollEvent();

	player->update();


	if (map->exit != nullptr &&
		(player->getPosition().x >= map->exit->rect.left && player->getPosition().x <= map->exit->rect.left + map->exit->rect.width) &&
		(player->getPosition().y >= map->exit->rect.top && player->getPosition().y <= map->exit->rect.top + map->exit->rect.height))
	{
		changeMap();
	}

	if (map->viewFollow) updateView();

	if (console->isEnabled) updateConsole();
}

void Game::updatePollEvent()
{
	while (window.pollEvent(this->e))
	{
		if (e.type == sf::Event::Closed)
		{
			window.close();
		}
		if (e.type == sf::Event::KeyPressed && e.key.code == sf::Keyboard::F1)
		{
			map->hitBoxesVisible = !map->hitBoxesVisible;
			player->showHitBox = !player->showHitBox;
		}
		if (e.type == sf::Event::KeyPressed && e.key.code == sf::Keyboard::F2)
		{
			map->originsVisible = !map->originsVisible;
			player->showOrigin = !player->showOrigin;
		}
		if (e.type == sf::Event::KeyPressed && e.key.code == sf::Keyboard::F3) 
		{
			onWindowResize();
		}
		if (e.type == sf::Event::KeyPressed && e.key.code == sf::Keyboard::Tilde)
		{
			console->isEnabled = !console->isEnabled;
		}
	}
}


void Game::updateView()
{
	sf::Vector2f shift(0.0f, 0.0f);

	if (player->getPosition().x + window.getSize().x / 2.0f < map->getActualBounds().x &&
		player->getPosition().x - window.getSize().x / 2.0f > 0.0f)
	{
		shift.x = player->getPosition().x - window.getView().getCenter().x;
	}

	if (player->getPosition().y + window.getSize().y / 2.0f < map->getActualBounds().y &&
		player->getPosition().y - window.getSize().y / 2.0f > 0.0f)
	{
		shift.y = player->getPosition().y - window.getView().getCenter().y;
	}

	window.setView(sf::View(sf::Vector2f(shift.x + window.getView().getCenter().x, shift.y + window.getView().getCenter().y), sf::Vector2f(window.getSize())));
}

void Game::updateConsole()
{
	console->setOutput(player->getFrameLog());
	console->setOutput("DeltaTime: " + std::to_string(deltaTime) + "\n");
}

void Game::changeMap()
{
	window.setView(window.getDefaultView());
	std::string nextMap = map->exit->nextMap;

	delete map;
	map = new Map(nextMap.c_str());
	map->onWindowResize(sf::Vector2f(window.getSize().x / resolution._default.x, window.getSize().y / resolution._default.y));
	player->setPosition((player->getPosition().x > map->exit->rect.left) ?
		map->exit->rect.left + map->exit->rect.width + 5.0f :
		map->exit->rect.left - 5.0f,
		map->exit->rect.top + map->exit->rect.height);
	player->showHitBox = false;
	player->showOrigin = false;
	player->collider->mapChange(*map->foregroundTiles);

	if (map->viewFollow) focusView();
}

void Game::focusView()
{
	sf::View view(player->getPosition(), sf::Vector2f(window.getSize()));
	sf::Vector2f center = player->getPosition();
	sf::Vector2f halfWindow(window.getSize().x / 2.0f, window.getSize().y / 2.0f);
	if (player->getPosition().x < halfWindow.x) center.x = halfWindow.x;
	else if (player->getPosition().x > map->getActualBounds().x - halfWindow.x) center.x = map->getActualBounds().x - halfWindow.x;
	if (player->getPosition().y < halfWindow.y) center.y = halfWindow.y;
	else if (player->getPosition().y > map->getActualBounds().y - halfWindow.y) center.y = map->getActualBounds().y - halfWindow.y;
	view.setCenter(center);
	window.setView(view);
}

//Render

void Game::render()
{
	window.clear();

	window.draw(*map);
	window.draw(*player);
	if (console->isEnabled) {
		sf::View tmp = window.getView();
		window.setView(window.getDefaultView());
		window.draw(*console);
		window.setView(tmp);
	}

	window.display();
}


void Game::onWindowResize()
{
	sf::Vector2f scale(resolution.fullScreen.x / window.getSize().x, resolution.fullScreen.y / window.getSize().y);
	map->onWindowResize(scale);
	player->onWindowResize(2.0f * scale);
	console->onWindowResize(scale);
	if (scale == sf::Vector2f(1.0f, 1.0f))
	{
		window.create(sf::VideoMode(resolution._default.x, resolution._default.y), "Game", sf::Style::Close);
	}
	else
	{
		window.create(sf::VideoMode(resolution.fullScreen.x, resolution.fullScreen.y), "Game", sf::Style::Fullscreen);
	}

	if (map->viewFollow) focusView();
}
