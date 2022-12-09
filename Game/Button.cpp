#include "Button.h"
#include "MainMenu.h"

Button::Button(sf::RenderWindow& window, sf::Font& font, sf::Vector2i size, sf::Texture& texture) : Entity(
	sf::Vector2f(size), texture), window(window)
{
	sprite.setTextureRect(sf::IntRect(sf::Vector2i(0, 0), size));
	sprite.setColor(sf::Color::Red);
	charSize = 30;
	this->text.setFont(font);
	showHitBox = true;
}

void Button::setCharacterSize(unsigned int size)
{
	text.setCharacterSize(size);
	charSize = size;
}

void Button::setFillColor(sf::Color& color)
{
	text.setFillColor(color);
}

void Button::setString(std::string str)
{
	text.setString(str);
}

void Button::setPosition(float x, float y)
{
	Entity::setPosition(x, y);
	text.setPosition(x, y);
}

void Button::setPosition(sf::Vector2f pos)
{
	setPosition(pos.x, pos.y);
}

void Button::setScale(float x, float y)
{
	Entity::setScale(x, y);
	text.setCharacterSize(charSize * x);
}

void Button::setScale(sf::Vector2f pos)
{
	setScale(pos.x, pos.y);
}

void Button::setPressHandler(std::function<void()> handler)
{
	this->handler = handler;
}

void Button::update()
{
	sprite.setColor(sf::Color::Red);
	sf::Vector2i mousePos = sf::Mouse::getPosition(dynamic_cast<sf::Window&>(window));
	if (mousePos.x < m_position.x || mousePos.x > m_position.x + m_actualSize.x) return;
	if (mousePos.y < m_position.y || mousePos.y > m_position.y + m_actualSize.y) return;
	sprite.setColor(sf::Color::Black);
	if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) handler();
}

void Button::onWindowResize(sf::Vector2f scale)
{
	Entity::onWindowResize(scale);
	text.setCharacterSize(charSize * scale.x);
}

void Button::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	Entity::draw(target, states);
	target.draw(text);
}
