#pragma once

#include <YOBA/System.hpp>

#if defined(YOBA_SYSTEM_SFML)

#include <cstdint>
#include <span>

#include <YOBA/Rendering/Targets/RenderingTarget.hpp>

#include <SFML/Graphics.hpp>

namespace YOBA {
	/**
	* @brief Encapsulates SFML sprite that will be used by YOBA for flushing pixel data.
	* The sprite itself can be rendered later via <b>window.draw()</b>
	*/
	class SFMLSpriteRenderingTarget : public RenderingTarget {
		public:
			void setup(
				const Size& size,
				const float renderingScale = 1.0f
			);

			const sf::Sprite& getSprite() const;

			float getRenderingScale() const;

			void flush(const Rectangle& bounds, const std::span<uint8_t> pixelBuffer) override;

		private:
			sf::Texture _texture {};
			uint8_t* _textureBuffer = nullptr;
			size_t _textureBufferLength = 0;

			sf::Sprite _sprite {
				_texture
			};

			using RenderingTarget::setup;
	};
}

#endif