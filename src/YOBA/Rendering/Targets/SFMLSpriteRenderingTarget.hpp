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
			) {
				RenderingTarget::setup(
					size,
					Rotation::none,
					PixelOrder::XNormalYNormal,
					ColorModel::ARGB
				);

				if (!_texture.resize(sf::Vector2u(size.getWidth(), size.getHeight())))
					return;

				_sprite.setTexture(_texture, true);
				_sprite.setScale({ renderingScale, renderingScale });

				_textureBufferLength = size.getSquare() * 4;
				system::reallocate(_textureBuffer, _textureBufferLength);

				// Filling alpha
				std::fill_n(_textureBuffer, _textureBufferLength, 255);
			}

			const sf::Sprite& getSprite() const {
				return _sprite;
			}

			float getRenderingScale() const {
				return _sprite.getScale().x;
			}

			void flush(const Rectangle& bounds, const std::span<uint8_t> pixelBuffer) override {
				const auto pixelBufferPtr = pixelBuffer.data();

				for (size_t i = 0, j = 0; i < _textureBufferLength; i += 4, j += 3) {
					_textureBuffer[i]     =  pixelBufferPtr[j];
					_textureBuffer[i + 1] =  pixelBufferPtr[j + 1];
					_textureBuffer[i + 2] =  pixelBufferPtr[j + 2];

					// Already 0xFF
					// textureBuffer[i + 3] = 0xFF;
				}

				_texture.update(_textureBuffer);
			}

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