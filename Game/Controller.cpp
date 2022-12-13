#include "Controller.h"


/*  <Constructors/Destructors>  */


Controller::Controller()
{
	globalVolume = 100.0f;
	deltaTime = 0.0f;
	switchFlag = Switch_Flag::IDLE;


	window.create(sf::VideoMode(960, 540), "Game", sf::Style::Close);
	currentWindow = new MainMenu(window, switchFlag, globalVolume);
}

Controller::~Controller()
{
	delete currentWindow;
}


/*  </Constructors/Destructors>  */


/*#################################################################################################################################################*/


/*  <Accessors>  */

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



/*  </Accessors>  */


/*#################################################################################################################################################*/


/*  <Update>  */


void Controller::update()
{
	currentWindow->update();

	if (switchFlag != Switch_Flag::IDLE) switchWindow();
}

void Controller::switchWindow()
{
	if (switchFlag == Switch_Flag::EXIT) return;

	delete currentWindow;

	if (switchFlag == Switch_Flag::GAME)
	{
		currentWindow = new Game(window, switchFlag, deltaTime);
	}
	else if (switchFlag == Switch_Flag::MAIN_MENU)
	{
		window.setView(window.getDefaultView());
		currentWindow = new MainMenu(window, switchFlag, globalVolume);
	}

	switchFlag = Switch_Flag::IDLE;
}


/*  </Update>  */


/*#################################################################################################################################################*/


/*  <Render>  */


void Controller::render()
{
	window.clear();

	window.draw(*currentWindow);

	window.display();
}


/*  </Render>  */