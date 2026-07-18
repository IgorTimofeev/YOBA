#include <YOBA/System.hpp>

#if defined(YOBA_SYSTEM_SFML)

#include <YOBA/Rendering/Targets/SFMLSpriteRenderingTarget.hpp>

namespace YOBA {
	void SFMLSpriteRenderingTarget::setup(const Size& size, const float renderingScale) {
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

	const sf::Sprite& SFMLSpriteRenderingTarget::getSprite() const {
		return _sprite;
	}

	float SFMLSpriteRenderingTarget::getRenderingScale() const {
		return _sprite.getScale().x;
	}

	void SFMLSpriteRenderingTarget::flush(const Rectangle& bounds, const std::span<uint8_t> pixelBuffer) {
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
}

#endif