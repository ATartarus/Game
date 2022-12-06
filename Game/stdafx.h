#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <SFML/Window.hpp>
#include <SFML/Network.hpp>
#include <SFML/System.hpp>
#include <iostream>


enum class Move_State : uint32_t
{
	IDLE = 0,
	MOVING_LEFT = 1,
	MOVING_RIGHT = (1 << 1),
	JUMPING = (1 << 2),
	FALLING = (1 << 3),
};

enum class Origin_Pos : uint32_t
{
	DEFAULT = 0,
	LEFT = 1,
	RIGHT = (1 << 1),
	TOP = (1 << 2),
	CENTER = (1 << 3),
	BOTTOM = (1 << 4),
};


constexpr inline bool operator==(const uint32_t& left, const Origin_Pos& right)
{
	return left == static_cast<uint32_t>(right);
}

constexpr inline Origin_Pos operator|(const Origin_Pos& left, const Origin_Pos& right)
{
	return static_cast<Origin_Pos>(static_cast<uint32_t>(left) | static_cast<uint32_t>(right));
}
