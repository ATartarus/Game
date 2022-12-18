#include "Game.h"


/*  <Constructors/Destructors>  */


Game::Game(sf::RenderWindow& window, Switch_Flag& flag, float globalVolume, const float& deltaTime) : Scene(window, flag),
	deltaTime(deltaTime)
{
	loadTextures();
	loadSounds();
	map = new Map("test.tmx");
	player = new Player(&textures["player"], *map->foregroundTiles, deltaTime);
	player->setPosition(80.0f, 500.0f);
	player->setInjuryBuffer(sounds["injury"]);
	player->setStepBuffer(sounds["step"]);
	player->setVolume(globalVolume);
	console = new Console();


	gameOverBackground.setTexture(textures["deathBackground"]);
	gameOverBackground.setScale(window.getSize().x / gameOverBackground.getGlobalBounds().width,
								window.getSize().y / gameOverBackground.getGlobalBounds().height);


	victoryBackground.setTexture(textures["victoryBackground"]);
	victoryBackground.setScale(window.getSize().x / gameOverBackground.getGlobalBounds().width,
		window.getSize().y / gameOverBackground.getGlobalBounds().height);



	sf::Vector2f scale = sf::Vector2f(window.getSize()) / resolution._default;
	if (m_contentScale != scale)
	{
		m_contentScale = scale;
		resizeContent(m_contentScale);
	}

	end = false;
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
	}

	textures.emplace("player", tmp);
	

	if (!tmp.loadFromFile("Texture\\deathScreen.png")) {
		std::cout << "Game::loadTextures could not load deathScreen.png" << "\n";
	}

	textures.emplace("deathBackground", tmp);


	if (!tmp.loadFromFile("Texture\\victoryScreen.png")) {
		std::cout << "Game::loadTextures could not load victoryScreen.png" << "\n";
	}

	textures.emplace("victoryBackground", tmp);


	if (!tmp.loadFromFile("Texture\\heart.png")) {
		std::cout << "Game::loadTextures could not load heart.png" << "\n";
	}

	textures.emplace("heart", tmp);
}

void Game::loadSounds()
{
	sf::SoundBuffer tmp;
	if (!tmp.loadFromFile("Audio\\injury.wav")) {
		std::cout << "MainMenu::loadSounds could not load injury.wav" << "\n";
	}
	sounds.emplace("injury", tmp);


	if (!tmp.loadFromFile("Audio\\step.wav")) {
		std::cout << "MainMenu::loadSounds could not load step.wav" << "\n";
	}
	sounds.emplace("step", tmp);
}


/*  </Constructors/Destructors>  */


/*#################################################################################################################################################*/


/*  <Update methods>  */


void Game::update()
{
	console->clearOutput();
	updateEvent();

	if (end) return;

	if (player->isAlive)
	{
		player->update();
		updateHearts();
	}

	if (map->exits) {
		for (auto& exit : *map->exits) {
			if ((player->getPosition().x >= exit.rect.left && player->getPosition().x <= exit.rect.left + exit.rect.width) &&
				(player->getPosition().y >= exit.rect.top && player->getPosition().y <= exit.rect.top + exit.rect.height))
			{
				if (exit.nextMap == "end") { end = true; return; }
				changeMap(exit);
				break;
			}
		}
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
		shift.x = (fabs(shift.x) < 1.0f) ? 0 : std::roundf(shift.x);
	}

	if (player->getPosition().y + window.getSize().y / 2.0f < map->getActualBounds().y &&
		player->getPosition().y - window.getSize().y / 2.0f > 0.0f)
	{
		shift.y = player->getPosition().y - window.getView().getCenter().y;
		shift.y = (fabs(shift.y) < 1.0f) ? 0 : std::roundf(shift.y);
	}

	window.setView(sf::View(sf::Vector2f(shift.x + window.getView().getCenter().x, shift.y + window.getView().getCenter().y), sf::Vector2f(window.getSize())));
}


void Game::updateConsole()
{
	console->setOutput(player->getFrameLog());
	console->setOutput("DeltaTime: " + std::to_string(deltaTime) + "\n");
}


void Game::updateHearts()
{
	hearts.clear();
	for (unsigned int i = player->getHP(); i > 0; i--)
	{
		sf::Sprite tmp(textures["heart"]);
		tmp.setScale(m_contentScale);
		tmp.setPosition(window.getSize().x - i * (tmp.getGlobalBounds().width + 5), 5);
		hearts.push_back(tmp);
	}
}


/*  </Update methods>  */


/*#################################################################################################################################################*/


/*  <Utility>  */


void Game::changeMap(Map::Exit& exit)
{
	window.setView(window.getDefaultView());
	std::string nextMap = exit.nextMap;
	std::string prevMap = map->name;

	delete map;
	map = new Map(nextMap.c_str());

	Map::Exit* dest = nullptr;
	for (auto& ex : *map->exits) {
		if (ex.nextMap == prevMap) {
			dest = &ex;
			break;
		}
	}
	if (!dest) return;

	map->onWindowResize(sf::Vector2f(window.getSize().x / resolution._default.x, window.getSize().y / resolution._default.y));
	player->setPosition((player->getPosition().x > dest->rect.left) ?
		dest->rect.left + dest->rect.width + 5.0f :
		dest->rect.left - 5.0f,
		dest->rect.top + dest->rect.height / 2.0f);
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
	player->onWindowResize(scale);
	console->onWindowResize(scale);
	gameOverBackground.setScale(scale);
	victoryBackground.setScale(scale);
}


void Game::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	target.draw(*map);
	target.draw(*player);
	if (end) {
		sf::View tmp = window.getView();
		window.setView(window.getDefaultView());
		target.draw(victoryBackground);
		window.setView(tmp);
		return;
	}
	if (!player->isAlive)
	{
		sf::View tmp = window.getView();
		window.setView(window.getDefaultView());
		target.draw(gameOverBackground);
		window.setView(tmp);
	}
	else 
	{
		sf::View tmp = window.getView();
		window.setView(window.getDefaultView());
		if (console->isEnabled) target.draw(*console);
		for (auto& hp : hearts) target.draw(hp);
		window.setView(tmp);
	}
}


/*  </Render>  */


/*#################################################################################################################################################*/