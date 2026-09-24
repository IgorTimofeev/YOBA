#include <YOBA/System.hpp>

#ifdef YOBA_SYSTEM_SFML

#include <YOBA/Rendering/Targets/SFMLRenderingTarget.hpp>

namespace YOBA {
	void SFMLRenderingTarget::setup(const Size& size) {
		RenderingTarget::setup(
			size,
			Rotation::none,
			PixelOrder::XNormalYNormal,
			ColorModel::ARGB
		);

		updateTextureSize();
	}

	sf::RenderTexture& SFMLRenderingTarget::getRenderTexture() {
		return _renderTexture;
	}

	sf::Sprite& SFMLRenderingTarget::getSprite() {
		return _sprite;
	}

	float SFMLRenderingTarget::getRenderingScale() const {
		return _sprite.getScale().x;
	}

	void SFMLRenderingTarget::setRenderingScale(const float value) {
		_sprite.setScale(sf::Vector2f(value, value));
	}

	void SFMLRenderingTarget::flush(const Rectangle& bounds, const std::span<uint8_t> pixelBuffer) {
		_renderTexture.display();
		_sprite.setTexture(_renderTexture.getTexture(), true);
	}

	void SFMLRenderingTarget::onDefaultSizeChanged() {
		updateTextureSize();
	}

	void SFMLRenderingTarget::onRotationChanged() {
		updateTextureSize();
	}

	void SFMLRenderingTarget::updateTextureSize() {
		_renderTexture.resize(sf::Vector2u(getSize().getWidth(), getSize().getHeight()));
	}
}

#endif