#include "MainMenu.h"


MainMenu::MainMenu(sf::RenderWindow& window, Switch_Flag& flag) : Scene(window, flag)
{
	loadTextures();
	initButtons();


	background.setTexture(textures["background"]);
	background.setScale(window.getSize().x / background.getGlobalBounds().width,
						window.getSize().y / background.getGlobalBounds().height);
}

MainMenu::~MainMenu()
{
}


void MainMenu::loadTextures()
{
	sf::Texture tmp;
	if (!tmp.loadFromFile("Texture\\mainMenu.png")) {
		std::cout << "MainMenu::loadTextures could not load mainMenu.png" << "\n";
		return;
	}
	textures.emplace("background", tmp);
}


void MainMenu::initButtons()
{
	if (!font.loadFromFile("Fonts\\arial.ttf")) {
		std::cout << "MainMenu::MainMenu could not load font" << "\n";
	}

	sf::Vector2f center = sf::Vector2f(window.getSize().x / 2.0f, window.getSize().y / 2.0f);

	Button* btn = new Button(window, font, sf::Vector2i(150, 40), textures["empty"]);
	btn->setString("Start");
	btn->setPosition(center.x - 75, window.getSize().y / 3.0f - 20);
	btn->setPressHandler([this] { switchFlag = Switch_Flag::GAME; });
	buttons.push_back(*btn);
	delete btn;

	btn = new Button(window, font, sf::Vector2i(150, 40), textures["empty"]);
	btn->setString("Exit");
	btn->setPosition(center.x - 75, window.getSize().y * (2.0f / 3.0f) - 20);
	btn->setPressHandler([this] { switchFlag = Switch_Flag::EXIT; e.type = sf::Event::Closed; });
	buttons.push_back(*btn);
	delete btn;
}


void MainMenu::updateEvent()
{
	while (window.pollEvent(e))
	{
		if (e.type == sf::Event::Closed || switchFlag == Switch_Flag::EXIT)
		{
			window.close();
		}
		if (e.type == sf::Event::KeyPressed && e.key.code == sf::Keyboard::F3)
		{
			onWindowResize();
		}
		if (e.type == sf::Event::KeyPressed && e.key.code == sf::Keyboard::Enter)
		{
			switchFlag = Switch_Flag::GAME;
		}
	}
}

void MainMenu::update()
{
	for (auto& btn : buttons) {
		btn.update();
	}
	updateEvent();
}

void MainMenu::onWindowResize()
{
	Scene::onWindowResize();
	background.setScale(m_contentScale);
	for (auto& btn : buttons) {
		btn.onWindowResize(m_contentScale);
	}
}

void MainMenu::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	target.draw(background);
	for (auto& btn : buttons) {
		target.draw(btn);
	}
}
