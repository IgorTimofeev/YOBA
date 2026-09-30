#include <YOBA/System.hpp>

#ifdef YOBA_SYSTEM_SFML

#include <YOBA/Rendering/Renderers/SFMLRenderer.hpp>

namespace YOBA {
	void SFMLRenderer::flush() {
		// TODO: rework this shit
		uint8_t pizda = 0;
		_target->flush(Rectangle(_target->getSize()), std::span(&pizda, 1));
	}

	void SFMLRenderer::clearNative(const Color* color) {
		auto& renderTexture = reinterpret_cast<SFMLRenderingTarget*>(_target)->getRenderTexture();
		renderTexture.clear(static_cast<const ARGBColor*>(color)->toSFMLColor());
	}

	void SFMLRenderer::fillRectangleNative(const Rectangle& bounds, const Color* color) {
		auto& renderTexture = reinterpret_cast<SFMLRenderingTarget*>(_target)->getRenderTexture();
		const auto sfmlColor = static_cast<const ARGBColor*>(color)->toSFMLColor();

		const std::array vertices {
			sf::Vertex(sf::Vector2f(bounds.getX(), bounds.getY()), sfmlColor),
			sf::Vertex(sf::Vector2f(bounds.getX() + bounds.getWidth(), bounds.getY()), sfmlColor),
			sf::Vertex(sf::Vector2f(bounds.getX(), bounds.getY() + bounds.getHeight()), sfmlColor),
			sf::Vertex(sf::Vector2f(bounds.getX() + bounds.getWidth(), bounds.getY() + bounds.getHeight()), sfmlColor)
		};

		renderTexture.draw(vertices.data(), vertices.size(), sf::PrimitiveType::TriangleStrip);
	}

	void SFMLRenderer::putImageNative(const Rectangle& bounds, const Image* image) {
		auto& renderTexture = reinterpret_cast<SFMLRenderingTarget*>(_target)->getRenderTexture();
		const auto sfmlImage = reinterpret_cast<const SFMLImage*>(image);

		if (!sfmlImage->getSprite())
			return;

		sfmlImage->getSprite()->setPosition(sf::Vector2f(bounds.getX(), bounds.getY()));

		sfmlImage->getSprite()->setScale(sf::Vector2f(
			static_cast<float>(bounds.getWidth()) / sfmlImage->getSprite()->getTexture().getSize().x,
			static_cast<float>(bounds.getHeight()) / sfmlImage->getSprite()->getTexture().getSize().y
		));

		renderTexture.draw(*sfmlImage->getSprite());
	}

	void SFMLRenderer::putPixelNative(const Point& position, const Color* color) {
		auto& renderTexture = reinterpret_cast<SFMLRenderingTarget*>(_target)->getRenderTexture();
		const auto sfmlColor = static_cast<const ARGBColor*>(color)->toSFMLColor();

		const std::array vertices {
			sf::Vertex(sf::Vector2f(position.getX(), position.getY()), sfmlColor),
			sf::Vertex(sf::Vector2f(position.getX() + 1, position.getY()), sfmlColor),
			sf::Vertex(sf::Vector2f(position.getX(), position.getY() + 1), sfmlColor),
			sf::Vertex(sf::Vector2f(position.getX() + 1, position.getY() + 1), sfmlColor)
		};

		renderTexture.draw(vertices.data(), vertices.size(), sf::PrimitiveType::TriangleStrip);
	}

	void SFMLRenderer::strokeHorizontalLineNative(const Point& position, const uint16_t length, const Color* color) {
		auto& renderTexture = reinterpret_cast<SFMLRenderingTarget*>(_target)->getRenderTexture();
		const auto sfmlColor = static_cast<const ARGBColor*>(color)->toSFMLColor();

		const std::array vertices {
			sf::Vertex(sf::Vector2f(position.getX(), position.getY()), sfmlColor),
			sf::Vertex(sf::Vector2f(position.getX() + length, position.getY()), sfmlColor),
			sf::Vertex(sf::Vector2f(position.getX(), position.getY() + 1), sfmlColor),
			sf::Vertex(sf::Vector2f(position.getX() + length, position.getY() + 1), sfmlColor)
		};

		renderTexture.draw(vertices.data(), vertices.size(), sf::PrimitiveType::TriangleStrip);
	}

	void SFMLRenderer::strokeVerticalLineNative(const Point& position, const uint16_t length, const Color* color) {
		auto& renderTexture = reinterpret_cast<SFMLRenderingTarget*>(_target)->getRenderTexture();
		const auto sfmlColor = static_cast<const ARGBColor*>(color)->toSFMLColor();

		const std::array vertices {
			sf::Vertex(sf::Vector2f(position.getX(), position.getY()), sfmlColor),
			sf::Vertex(sf::Vector2f(position.getX(), position.getY() + length), sfmlColor),
			sf::Vertex(sf::Vector2f(position.getX() + 1, position.getY()), sfmlColor),
			sf::Vertex(sf::Vector2f(position.getX() + 1, position.getY() + length), sfmlColor),
		};

		renderTexture.draw(vertices.data(), vertices.size(), sf::PrimitiveType::TriangleStrip);
	}
}

#endif