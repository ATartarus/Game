#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <SFML/Window.hpp>
#include <SFML/Network.hpp>
#include <SFML/System.hpp>
#include <iostream>

enum Player_State
{
	IDLE = 0,
	MOVING_LEFT = 1,
	MOVING_RIGHT = (1 << 1),
	JUMPING = (1 << 2),
	FALLING = (1 << 3),
};

enum Origin_Pos {
	DEFAULT = 0,
	LEFT = 1,
	RIGHT = 2,
	TOP = 4,
	CENTER = 8,
	BOTTOM = 16,
};