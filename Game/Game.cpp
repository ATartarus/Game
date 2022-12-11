#include "Game.h"


/*  <Constructors/Destructors>  */


Game::Game(sf::RenderWindow& window, Switch_Flag& flag, const float& deltaTime) : Scene(window, flag),
	deltaTime(deltaTime)
{
	loadTextures();
	map = new Map("newtest.tmx");
	player = new Player(textures["player"], *map->foregroundTiles, deltaTime);
	player->setPosition(200.0f, 200.0f);
	console = new Console();


	gameOverBackground.setTexture(textures["background"]);
	gameOverBackground.setScale(window.getSize().x / gameOverBackground.getGlobalBounds().width,
								window.getSize().y / gameOverBackground.getGlobalBounds().height);
	gameOverBackground.setColor(sf::Color(255, 0, 0, 155));

	sf::Vector2f scale = sf::Vector2f(window.getSize()) / resolution._default;
	if (m_contentScale != scale)
	{
		m_contentScale = scale;
		resizeContent(m_contentScale);
	}
}


Game::~Game()
{
	delete map;
	delete player;
	delete console;
}


void Game::loadTextures()
{
	sf::Texture tmp;

	if (!tmp.loadFromFile("Texture\\player_sheet.png")) {
		std::cout << "Game::loadTextures could not load player_sheet.png" << "\n";
		return;
	}

	textures.emplace("player", tmp);
	

	if (!tmp.loadFromFile("Texture\\deathScreen.png")) {
		std::cout << "Game::loadTextures could not load deathScreen.png" << "\n";
		return;
	}

	textures.emplace("background", tmp);
}

void Game::loadSounds()
{

}


/*  </Constructors/Destructors>  */


/*#################################################################################################################################################*/


/*  <Update methods>  */


void Game::update()
{
	console->clearOutput();

	updateEvent();

	if (player->isAlive) player->update();

	if (map->exit != nullptr &&
		(player->getPosition().x >= map->exit->rect.left && player->getPosition().x <= map->exit->rect.left + map->exit->rect.width) &&
		(player->getPosition().y >= map->exit->rect.top && player->getPosition().y <= map->exit->rect.top + map->exit->rect.height))
	{
		changeMap();
	}

	if (map->viewFollow) updateView();

	if (console->isEnabled) updateConsole();
}


void Game::updateEvent()
{
	while (window.pollEvent(e))
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
		if (e.type == sf::Event::KeyPressed && e.key.code == sf::Keyboard::Escape)
		{
			switchFlag = Switch_Flag::MAIN_MENU;
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



/*  </Update methods>  */


/*#################################################################################################################################################*/


/*  <Utility>  */


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


/*  </Utility>  */


/*#################################################################################################################################################*/


/*  <Render>  */


void Game::onWindowResize()
{
	Scene::onWindowResize();
	resizeContent(m_contentScale);

	if (map->viewFollow) focusView();
}


void Game::resizeContent(sf::Vector2f scale)
{
	map->onWindowResize(scale);
	player->onWindowResize(2.0f * scale);
	console->onWindowResize(scale);
	gameOverBackground.setScale(scale);
}


void Game::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	target.draw(*map);
	target.draw(*player);
	if (console->isEnabled)
	{
		sf::View tmp = window.getView();
		window.setView(window.getDefaultView());
		target.draw(*console);
		window.setView(tmp);
	}
	if (!player->isAlive)
	{
		sf::View tmp = window.getView();
		window.setView(window.getDefaultView());
		target.draw(gameOverBackground);
		window.setView(tmp);
	}
}


/*  </Render>  */


/*#################################################################################################################################################*/