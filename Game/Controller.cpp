#include "Controller.h"


//Cons/Dest


Controller::Controller()
{
	window.create(sf::VideoMode(960, 540), "Game", sf::Style::Close);
	currentWindow = new MainMenu(window, switchFlag);
	switchFlag = Switch_Flag::IDLE;
	deltaTime = 0.0f;
}

Controller::~Controller()
{
	delete currentWindow;
}

//Accessors

const sf::RenderWindow& Controller::getWindow() const
{
	return window;
}

const float Controller::getDeltaTime() const
{
	return deltaTime;
}

void Controller::setDeltaTime(const float deltaTime)
{
	this->deltaTime = deltaTime;
}


//Update

void Controller::update()
{
	currentWindow->update();

	if (switchFlag != Switch_Flag::IDLE) switchWindow();
}

void Controller::switchWindow()
{
	delete currentWindow;


	if (switchFlag == Switch_Flag::GAME)
	{
		currentWindow = new Game(window, switchFlag, deltaTime);
	}
	else if (switchFlag == Switch_Flag::MAIN_MENU)
	{
		window.setView(window.getDefaultView());
		currentWindow = new MainMenu(window, switchFlag);
	}

	switchFlag = Switch_Flag::IDLE;
}

//Render

void Controller::render()
{
	window.clear();

	window.draw(*currentWindow);

	window.display();
}
