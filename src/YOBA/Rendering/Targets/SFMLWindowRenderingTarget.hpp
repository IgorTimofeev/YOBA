#pragma once

#include <YOBA/System.hpp>

#if defined(YOBA_SYSTEM_DESKTOP)

#include <cstdint>
#include <span>

#include <YOBA/Rendering/Targets/RenderingTarget.hpp>

#include <SFML/Graphics.hpp>

namespace YOBA {
	class SFMLWindowRenderingTarget : public RenderingTarget {
		public:
			void setup(
				sf::RenderWindow* window,
				const Size& size,
				const float renderingScale
			) {
				_window = window;
				_renderingScale = renderingScale;

				RenderingTarget::setup(
					size,
					Rotation::none,
					PixelOrder::XNormalYNormal,
					ColorModel::RGB888
				);

				if (!_texture.resize(sf::Vector2u(size.getWidth(), size.getHeight())))
					return;

				_sprite.setTexture(_texture, true);
				_sprite.setScale({ _renderingScale, _renderingScale });

				_textureBufferLength = size.getSquare() * 4;
				system::reallocate(_textureBuffer, _textureBufferLength);
				std::fill_n(_textureBuffer, _textureBufferLength, 255);
			}

			sf::RenderWindow* getWindow() const {
				return _window;
			}

			float getRenderingScale() const {
				return _renderingScale;
			}

			void flush(const Rectangle& bounds, const std::span<uint8_t> pixelBuffer) override {
				const auto pixelBufferPtr = pixelBuffer.data();

				for (size_t i = 0, j = 0; i < _textureBufferLength; i += 4, j += 3) {
					_textureBuffer[i]     =  pixelBufferPtr[j];
					_textureBuffer[i + 1] =  pixelBufferPtr[j + 1];
					_textureBuffer[i + 2] =  pixelBufferPtr[j + 2];
					// textureBuffer[i + 3] = 0xFF;
				}

				_texture.update(_textureBuffer);
				_window->draw(_sprite);
			}

		private:
			sf::RenderWindow* _window = nullptr;
			float _renderingScale = 1;

			sf::Texture _texture {};
			uint8_t* _textureBuffer = nullptr;
			size_t _textureBufferLength = 0;

			sf::Sprite _sprite { _texture };

			using RenderingTarget::setup;
	};
}

#endif