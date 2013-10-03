#include "Marker.hpp"

#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/RenderTarget.hpp>

#include <iostream>

Marker::Marker(std::shared_ptr<sf::Texture> texture, const sf::IntRect& rect, const sf::Color& color, const sf::Vector2f& position)
{
	if(texture != nullptr)
		m_sprite.setTexture(*texture);
	else
		std::cout << "Bad Texture!!" << std::endl;

	m_sprite.setTextureRect(rect);
	m_sprite.setColor(color);
	m_sprite.setOrigin(m_sprite.getLocalBounds().width/2, m_sprite.getLocalBounds().height/2);
	m_sprite.setPosition(position);

}

void Marker::position(const sf::Vector2f& pos)
{
	m_sprite.setPosition(pos);
}

void Marker::draw(sf::RenderTarget &target, sf::RenderStates states) const
{
	target.draw(m_sprite, states);
}
