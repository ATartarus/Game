#include "Slider.h"


/*  <Constructors/Destructors>  */


Slider::Slider(sf::RenderWindow& window, sf::Vector2f lineSize, sf::Vector2f thumbSize, float& value, float maxValue, sf::Texture* lineTexture, sf::Texture* thumbTexture) : Entity(
	lineSize, lineTexture, sf::IntRect(0, 0, lineSize.x, lineSize.y)), thumb(
	thumbSize, thumbTexture, sf::IntRect(0, 0, thumbSize.x, thumbSize.y), Origin_Pos::CENTER),
	window(window), bindedValue(value)
{
	tick = lineSize.x / maxValue;

	thumb.setPosition(tick * bindedValue, lineSize.y / 2.0f);


	leftPressed = false;
	sound1.setVolume(bindedValue);
	sound2.setVolume(bindedValue);
	sound3.setVolume(bindedValue);
}


/*  </Constructors/Destructors>  */

/*  <Accessors>  */


void Slider::setPosition(float x, float y)
{
	Entity::setPosition(x, y);
	thumb.setPosition(x + tick * bindedValue, y + m_actualSize.y / 2.0f);
}

void Slider::setPosition(sf::Vector2f pos)
{
	setPosition(pos.x, pos.y);
}

void Slider::setScale(float x, float y)
{
	Entity::setScale(x, y);
	thumb.setScale(x, y);
}

void Slider::setScale(sf::Vector2f scale)
{
	setScale(scale.x, scale.y);
}

void Slider::setDragHandler(std::function<void()> handler)
{
	this->dragHandler = handler;
}

void Slider::setSoundBuffer(const sf::SoundBuffer& buffer)
{
	sound1.setBuffer(buffer);
	sound2.setBuffer(buffer);
	sound3.setBuffer(buffer);
}


/*  </Accessors>  */

/*  <Update>  */


void Slider::update()
{
	sf::Vector2i mousePos = sf::Mouse::getPosition(dynamic_cast<sf::Window&>(window));

	if (leftPressed) {
		if (mousePos.x > m_position.x + m_actualSize.x)	thumb.setPosition(m_position.x + m_actualSize.x, thumb.getPosition().y);
		else if (mousePos.x < m_position.x)				thumb.setPosition(m_position.x, thumb.getPosition().y);
		else											thumb.setPosition(mousePos.x, thumb.getPosition().y);

		double delta = std::round(bindedValue * tick - (thumb.getPosition().x - m_position.x));
		if (fabs(delta) >= tick) 
		{
			bindedValue += (delta > 0) ? -1 : 1;
			thumb.setPosition(bindedValue * tick + m_position.x, thumb.getPosition().y);
			sound1.setVolume(bindedValue);
			sound2.setVolume(bindedValue);
			sound3.setVolume(bindedValue);
			if (sound1.getStatus() == sf::SoundSource::Stopped) sound1.play();
			else if (sound2.getStatus() == sf::SoundSource::Stopped) sound2.play();
			else if (sound3.getStatus() == sf::SoundSource::Stopped) sound3.play();
			dragHandler();
		}
	}	

	if (leftPressed && !sf::Mouse::isButtonPressed(sf::Mouse::Left)) leftPressed = false;
	if (mousePos.x < thumb.getPosition().x - thumb.getActualBounds().x / 2.0f || mousePos.x > thumb.getPosition().x + thumb.getActualBounds().x / 2.0f) { return; }
	if (mousePos.y < thumb.getPosition().y - thumb.getActualBounds().y / 2.0f || mousePos.y > thumb.getPosition().y + thumb.getActualBounds().y / 2.0f) { return; }
	if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) leftPressed = true;
}


/*  </Update>  */

/*  <Render>  */


void Slider::onWindowResize(sf::Vector2f scale)
{
	tick *= scale.x / fabs(m_scale.x);
	Entity::onWindowResize(scale);
}

void Slider::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	Entity::draw(target, states);
	target.draw(thumb);
}


/*  </Render>  */