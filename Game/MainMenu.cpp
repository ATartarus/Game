#include "MainMenu.h"


/*  <Constructors/Destructors>  */


MainMenu::MainMenu(sf::RenderWindow& window, Switch_Flag& flag) : Scene(window, flag)
{
	loadTextures();
	loadSounds();
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
	if (!tmp.loadFromFile("Texture\\2.png")) {
		std::cout << "MainMenu::loadTextures could not load mainMenu.png" << "\n";
	}
	textures.emplace("background", tmp);
}

void MainMenu::loadSounds()
{
	sf::SoundBuffer tmp;
	if (!tmp.loadFromFile("Audio\\button.wav")) {
		std::cout << "MainMenu::loadSounds could not load button.wav" << "\n";
	}
	sounds.emplace("button", tmp);
}


void MainMenu::initButtons()
{
	if (!font.loadFromFile("Fonts\\ARCADECLASSIC.ttf")) {
		std::cout << "MainMenu::MainMenu could not load font" << "\n";
	}

	sf::Vector2f center = sf::Vector2f(window.getSize().x / 2.0f, window.getSize().y / 2.0f);

	Button* btn = new Button(window, font, sf::Vector2i(128, 40), textures["empty"]);
	btn->setString("Play");
	btn->setPosition(50.0f, 50.0f);
	btn->setPressHandler([this] { switchFlag = Switch_Flag::GAME; });
	btn->setSoundBuffer(sounds["button"]);
	buttons.push_back(*btn);
	delete btn;

	btn = new Button(window, font, sf::Vector2i(128, 40), textures["empty"]);
	btn->setString("Exit");
	btn->setPosition(50.0f, 270.0f);
	btn->setPressHandler([this] { switchFlag = Switch_Flag::EXIT; });
	btn->setSoundBuffer(sounds["button"]);
	buttons.push_back(*btn);
	delete btn;
}


/*  </Constructors/Destructors>  */


/*#################################################################################################################################################*/


/*  <Update>  */


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


/*  </Update>  */


/*#################################################################################################################################################*/


/*  <Render>  */


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


/*  </Render>  */


/*#################################################################################################################################################*/