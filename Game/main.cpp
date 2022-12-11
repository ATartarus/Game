#include "stdafx.h"
#include "Controller.h"

int main()
{
    Controller control;
    sf::Clock clock;
  

    while (control.getWindow().isOpen()) 
    {
        control.setDeltaTime(clock.restart().asSeconds());
        if (control.getDeltaTime() > 1.0f / 40.0f) control.setDeltaTime(1.0f / 40.0f);
        control.update();
        control.render();
    }

    return 0;
}

