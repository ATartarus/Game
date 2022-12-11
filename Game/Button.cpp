#include "Button.h"
#include "MainMenu.h"


/*  <Constructors/Destructors>  */


Button::Button(sf::RenderWindow& window, sf::Font& font, sf::Vector2i size, sf::Texture& texture) : Entity(
	sf::Vector2f(size), texture), window(window)
{
	sprite.setTextureRect(sf::IntRect(sf::Vector2i(0, 0), size));
	sprite.setColor(sf::Color::Transparent);
	charSize = 60;
	text.setCharacterSize(charSize);
	this->text.setFont(font);
	leftPressed = false;
	hovered = false;

	text.setOutlineThickness(2);
}


/*  </Constructors/Destructors>  */


/*#################################################################################################################################################*/


/*  <Accessors>  */


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
	text.setPosition(x, y - text.getGlobalBounds().height / 2.0f);
}

void Button::setPosition(sf::Vector2f pos)
{
	setPosition(pos.x, pos.y);
}

void Button::setScale(float x, float y)
{
	Entity::setScale(x, y);
}

void Button::setScale(sf::Vector2f pos)
{
	setScale(pos.x, pos.y);
}

void Button::setPressHandler(std::function<void()> handler)
{
	this->handler = handler;
}

void Button::setSoundBuffer(const sf::SoundBuffer& buffer)
{
	sound.setBuffer(buffer);
}


/*  </Accessors>  */


/*#################################################################################################################################################*/


/*  <Update>  */


void Button::update()
{
	text.setCharacterSize(charSize);
	text.setOutlineColor(sf::Color::Transparent);
	sf::Vector2i mousePos = sf::Mouse::getPosition(dynamic_cast<sf::Window&>(window));
	if (mousePos.x < m_position.x || mousePos.x > m_position.x + m_actualSize.x) { hovered = false; leftPressed = false; return; }
	if (mousePos.y < m_position.y || mousePos.y > m_position.y + m_actualSize.y) { hovered = false; leftPressed = false; return; }

	text.setCharacterSize(charSize + 2);
	text.setOutlineColor(sf::Color::Color(128, 128, 128));
	if (!hovered) sound.play();
	if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) leftPressed = true;
	if (leftPressed && !sf::Mouse::isButtonPressed(sf::Mouse::Left)) handler();

	hovered = true;
}


/*  </Update>  */


/*#################################################################################################################################################*/


/*  <Render>  */


void Button::onWindowResize(sf::Vector2f scale)
{
	text.setCharacterSize(charSize *= (scale.x / m_scale.x));
	Entity::onWindowResize(scale);
}

void Button::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	Entity::draw(target, states);
	target.draw(text);
}


/*  </Render>  */


/*#################################################################################################################################################*/