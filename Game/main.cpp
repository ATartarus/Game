#include "stdafx.h"
#include "Game.h"

int main()
{
    Game game;
    sf::Clock clock;
  

    while (game.getWindow().isOpen()) 
    {
        game.setDeltaTime(clock.restart().asSeconds());
        game.update();
        game.render();
    }

    return 0;
}

