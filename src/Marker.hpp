#pragma once

#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Graphics/RenderStates.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/System/Vector2.hpp>

#include <memory>

namespace sf
{
	class Color;
	class RenderTarget;
}

class Marker : public sf::Drawable
{
public:
	Marker(std::shared_ptr<sf::Texture> texture, const sf::IntRect& rect, const sf::Color& color, const sf::Vector2f& position);
	void position(const sf::Vector2f& pos);

private:
	virtual void draw(sf::RenderTarget &target, sf::RenderStates states) const;

private:
	sf::Sprite m_sprite;
};
