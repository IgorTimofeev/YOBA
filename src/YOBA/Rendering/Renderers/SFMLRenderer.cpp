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

	void SFMLRenderer::fillRectangleNative(const Rectangle& bounds, const Color* color) {
		auto& texture = reinterpret_cast<SFMLRenderingTarget*>(_target)->getRenderTexture();

		sf::RectangleShape shape(sf::Vector2f(bounds.getWidth(), bounds.getHeight()));
		shape.setPosition(sf::Vector2f(bounds.getX(), bounds.getY()));
		shape.setFillColor(static_cast<const ARGBColor*>(color)->toSFMLColor());
		texture.draw(shape);
	}

	void SFMLRenderer::putImageNative(const Rectangle& bounds, const Image* image) {
		auto& renderTexture = reinterpret_cast<SFMLRenderingTarget*>(_target)->getRenderTexture();
		const auto sfmlImage = reinterpret_cast<const SFMLImage*>(image);

		sfmlImage->getSprite()->setPosition(sf::Vector2f(bounds.getX(), bounds.getY()));

		sfmlImage->getSprite()->setScale(sf::Vector2f(
			static_cast<float>(bounds.getWidth()) / sfmlImage->getSprite()->getTexture().getSize().x,
			static_cast<float>(bounds.getHeight()) / sfmlImage->getSprite()->getTexture().getSize().y
		));

		renderTexture.draw(*sfmlImage->getSprite());
	}
}

#endif
