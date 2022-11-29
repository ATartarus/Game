#include "stdafx.h"
#include "Game.h"

int main()
{
    Game game;
    sf::Clock clock;
  

    while (game.getWindow().isOpen()) 
    {
        game.setDeltaTime(clock.restart().asSeconds());
        if (game.getDeltaTime() > 1.0f / 30.0f) game.setDeltaTime(1.0f / 30.0f);
        game.update();
        game.render();
    }

    return 0;
}

