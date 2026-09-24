#pragma once

#include <YOBA/System.hpp>

#ifdef YOBA_SYSTEM_SFML

#include <YOBA/Rendering/Renderers/SFMLRenderer.hpp>

namespace YOBA {
	void SFMLRenderer::flush() {
		// TODO: rework this shit
		uint8_t pizda = 0;
		_target->flush(Rectangle(_target->getSize()), std::span(&pizda, 1));
	}

	void SFMLRenderer::putSprite(const Point& position, sf::Sprite* sprite) const {
		auto& renderTexture = reinterpret_cast<SFMLRenderingTarget*>(_target)->getRenderTexture();

		sprite->setPosition(sf::Vector2f(position.getX(), position.getY()));
		renderTexture.draw(*sprite);
	}

	void SFMLRenderer::fillRectangleNative(const Rectangle& bounds, const Color* color) {
		auto& texture = reinterpret_cast<SFMLRenderingTarget*>(_target)->getRenderTexture();

		sf::RectangleShape shape(sf::Vector2f(bounds.getWidth(), bounds.getHeight()));
		shape.setPosition(sf::Vector2f(bounds.getX(), bounds.getY()));
		shape.setFillColor(static_cast<const ARGBColor*>(color)->toSFMLColor());
		texture.draw(shape);
	}

	void SFMLRenderer::putImageNative(const Point& position, const Image* image) {

	}
}

#endif
