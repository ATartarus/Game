#include "MainMenu.h"


/*  <Constructors/Destructors>  */


MainMenu::MainMenu(sf::RenderWindow& window, Switch_Flag& flag, float& globalVolume) : Scene(window, flag),
	globalVolume(globalVolume)
{
	if (!font.loadFromFile("Fonts\\ARCADECLASSIC.ttf")) {
		std::cout << "MainMenu::MainMenu could not load font" << "\n";
	}
	isInOptions = false;

	loadTextures();
	loadSounds();
	initMainMenu();


	sf::Vector2f scale = sf::Vector2f(window.getSize()) / resolution._default;
	if (m_contentScale != scale)
	{
		m_contentScale = scale;
		resizeContent();
	}
}

MainMenu::~MainMenu()
{
}

void MainMenu::loadTextures()
{
	sf::Texture tmp;
	if (!tmp.loadFromFile("Texture\\mainMenuBack.png")) {
		std::cout << "MainMenu::loadTextures could not load mainMenuBack.png" << "\n";
	}
	textures.emplace("mainBackground", tmp);


	if (!tmp.loadFromFile("Texture\\optionsBack.png")) {
		std::cout << "MainMenu::loadTextures could not load optionsBack.png" << "\n";
	}
	textures.emplace("optionsBackground", tmp);


	if (!tmp.loadFromFile("Texture\\thumb.png")) {
		std::cout << "MainMenu::loadTextures could not load thumb.png" << "\n";
	}
	textures.emplace("thumb", tmp);


	if (!tmp.loadFromFile("Texture\\slider.png")) {
		std::cout << "MainMenu::loadTextures could not load slider.png" << "\n";
	}
	textures.emplace("slider", tmp);
}

void MainMenu::loadSounds()
{
	sf::SoundBuffer tmp;
	if (!tmp.loadFromFile("Audio\\button.wav")) {
		std::cout << "MainMenu::loadSounds could not load button.wav" << "\n";
	}
	sounds.emplace("button", tmp);

	if (!tmp.loadFromFile("Audio\\slider.wav")) {
		std::cout << "MainMenu::loadSounds could not load slider.wav" << "\n";
	}
	sounds.emplace("slider", tmp);
}


void MainMenu::initMainMenu()
{
	isInOptions = false;
	buttons.clear();
	sliders.clear();
	labels.clear();

	background.setScale(1.0f, 1.0f);
	background.setTexture(textures["mainBackground"]);
	background.setScale(window.getSize().x / background.getGlobalBounds().width,
						window.getSize().y / background.getGlobalBounds().height);

	/// ---------------------------------------------------------------- Buttons

	Button* btn = new Button(window, font, sf::Vector2i(128, 40), nullptr);
	btn->setString("Play");
	btn->setPosition(50.0f, 50.0f);
	btn->setPressHandler([this] { switchFlag = Switch_Flag::GAME; });
	btn->setSoundBuffer(sounds["button"]);
	btn->setSoundVolume(globalVolume);
	buttons.push_back(*btn);
	delete btn;

	btn = new Button(window, font, sf::Vector2i(224, 40), nullptr);
	btn->setString("Options");
	btn->setPosition(50.0f, 160.0f);
	btn->setPressHandler([this] { initOptionsMenu(); });
	btn->setSoundBuffer(sounds["button"]);
	btn->setSoundVolume(globalVolume);
	buttons.push_back(*btn);
	delete btn;

	btn = new Button(window, font, sf::Vector2i(128, 40), nullptr);
	btn->setString("Exit");
	btn->setPosition(50.0f, 270.0f);
	btn->setPressHandler([this] { switchFlag = Switch_Flag::EXIT; });
	btn->setSoundBuffer(sounds["button"]);
	btn->setSoundVolume(globalVolume);
	buttons.push_back(*btn);
	delete btn;


	resizeContent();
}


void MainMenu::initOptionsMenu()
{
	isInOptions = true;
	buttons.clear();
	sliders.clear();
	labels.clear();

	background.setScale(1.0f, 1.0f);
	background.setTexture(textures["optionsBackground"]);
	background.setScale(window.getSize().x / background.getGlobalBounds().width,
		window.getSize().y / background.getGlobalBounds().height);

	/// ---------------------------------------------------------------- Buttons

	Button* btn = new Button(window, font, sf::Vector2i(330, 40), nullptr);
	btn->setString("FullScreen");
	btn->setCharacterSize(40);
	btn->setPosition(50.0f, 300.0f);
	btn->setPressHandler([this] { onWindowResize(); });
	btn->setSoundBuffer(sounds["button"]);
	btn->setSoundVolume(globalVolume);
	buttons.push_back(*btn);
	delete btn;

	/// ---------------------------------------------------------------- Labels

	sf::Text tmp;
	tmp.setFont(font);
	tmp.setString(std::to_string(static_cast<int>(globalVolume)));
	tmp.setPosition(550.0f, 85.f);
	labels.emplace("volume", tmp);
	
	tmp.setString("Audio");
	tmp.setCharacterSize(30);
	tmp.setPosition(50, 50);
	labels.emplace("audio", tmp);

	/// ---------------------------------------------------------------- Sliders

	Slider* slider = new Slider(window, sf::Vector2f(480, 5), sf::Vector2f(10, 20), globalVolume, 100, &textures["slider"], &textures["thumb"]);
	slider->setPosition(50, 100);
	slider->setDragHandler([this]() {
		for (auto& btn : buttons) { btn.setSoundVolume(globalVolume); }
		labels["volume"].setString(std::to_string(static_cast<int>(globalVolume)));
		});
	slider->setSoundBuffer(sounds["slider"]);
	sliders.push_back(*slider);
	delete slider;


	resizeContent();
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
		if (!isInOptions && e.type == sf::Event::KeyPressed && e.key.code == sf::Keyboard::Enter)
		{
			switchFlag = Switch_Flag::GAME;
		}
		if (isInOptions && e.type == sf::Event::KeyPressed && e.key.code == sf::Keyboard::Escape)
		{
			initMainMenu();
		}
	}
}

void MainMenu::update()
{
	for (auto& btn : buttons) {
		btn.update();
	}
	if (isInOptions) {
		for (auto& slide : sliders) {
			slide.update();
		}
	}
	updateEvent();
}


/*  </Update>  */


/*#################################################################################################################################################*/


/*  <Render>  */


void MainMenu::resizeContent()
{
	sf::Vector2f prevScale = buttons[0].getScale();

	background.setScale(m_contentScale);
	for (auto& btn : buttons) {
		btn.onWindowResize(m_contentScale);
	}
	for (auto& slide : sliders) {
		slide.onWindowResize(m_contentScale);
	}
	for (auto& label : labels) {
		label.second.setPosition(label.second.getPosition().x * (m_contentScale.x / prevScale.x),
			label.second.getPosition().y * (m_contentScale.y / prevScale.y));
		label.second.setCharacterSize(label.second.getCharacterSize() * (m_contentScale.x / prevScale.x));
	}
}


void MainMenu::onWindowResize()
{
	Scene::onWindowResize();
	resizeContent();
}


void MainMenu::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	target.draw(background);
	for (auto& btn : buttons) {
		target.draw(btn);
	}

	for (auto& slide : sliders) target.draw(slide);

	for (auto& label : labels) target.draw(label.second);
}


/*  </Render>  */


/*#################################################################################################################################################*/