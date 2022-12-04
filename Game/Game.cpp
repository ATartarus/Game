#include "Game.h"


//Cons/Dest

Game::Game() 
{
	window.create(sf::VideoMode(960, 540), "Game", sf::Style::Close);
	map = new Map("test.tmx");
	player = new Player();
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
	//std::cout << this->deltaTime << "\n";

	//int sum = 0;
	//for (int i = 0; i < 100000000; i++) {
	//	sum += i;
	//}
	console->clearOutput();

	updatePollEvent();

	updatePlayer();

	if (map->viewFollow) updateView();


	if (map->exit != nullptr &&
		(player->getPosition().x >= map->exit->rect.left && player->getPosition().x <= map->exit->rect.left + map->exit->rect.width) &&
		(player->getPosition().y >= map->exit->rect.top && player->getPosition().y <= map->exit->rect.top + map->exit->rect.height))
	{
		changeMap();
	}

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

void Game::updatePlayer()
{
	this->player->update(deltaTime);

	int Y1 = static_cast<int>((player->getPosition().y - player->getActualBounds().y) / map->getActualTileSize().y);
	int Y2 = static_cast<int>(player->getPosition().y / map->getActualTileSize().y + 1);
	int X1 = static_cast<int>((player->getPosition().x - player->getActualBounds().x / 2.0f) / map->getActualTileSize().x);
	int X2 = static_cast<int>((player->getPosition().x + player->getActualBounds().x / 2.0f) / map->getActualTileSize().x + 1);

	if (Y2 > map->foregroundTiles->size()) Y2 = static_cast<int>(map->foregroundTiles->size());
	if (X2 > (*map->foregroundTiles)[0].size()) X2 = static_cast<int>((*map->foregroundTiles)[0].size());

	for (int i = Y1; i < Y2; i++) {
		for (int j = X1; j < X2; j++) {
			if ((*map->foregroundTiles)[i][j] == nullptr) continue;
			player->updateCollision(*(*map->foregroundTiles)[i][j]);
		}
	}
}

void Game::updateView()
{
	if (player->getPosition().x + window.getSize().x / 2.0f < map->getActualBounds().x &&
		player->getPosition().x - window.getSize().x / 2.0f > 0.0f)
	{
		window.setView(sf::View(sf::Vector2f(player->getPosition().x, window.getView().getCenter().y), sf::Vector2f(window.getSize())));
	}

	if (player->getPosition().y + window.getSize().y / 2.0f < map->getActualBounds().y &&
		player->getPosition().y - window.getSize().y / 2.0f > 0.0f)
	{
		window.setView(sf::View(sf::Vector2f(window.getView().getCenter().x, player->getPosition().y), sf::Vector2f(window.getSize())));
	}
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
	map->setResolutionScale(sf::Vector2f(window.getSize().x / resolution._default.x, window.getSize().y / resolution._default.y));
	player->setPosition((player->getPosition().x > map->exit->rect.left) ?
		map->exit->rect.left + map->exit->rect.width + 5.0f :
		map->exit->rect.left - 5.0f,
		map->exit->rect.top + map->exit->rect.height);
	player->showHitBox = false;
	player->showOrigin = false;

	if (map->viewFollow) focusView();
}

void Game::focusView()
{
	sf::View view(player->getPosition(), sf::Vector2f(window.getSize()));
	sf::Vector2f center;
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
	map->setResolutionScale(scale);
	player->setResolutionScale(2.0f * scale);
	console->setResolutionScale(scale);
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
