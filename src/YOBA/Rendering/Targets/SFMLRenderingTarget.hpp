#pragma once

#include <YOBA/System.hpp>

#ifdef YOBA_SYSTEM_SFML

#include <cstdint>
#include <span>

#include <YOBA/Rendering/Targets/RenderingTarget.hpp>

#include <SFML/Graphics.hpp>

namespace YOBA {
	/**
	* @brief Encapsulates sf::Sprite that will be used by YOBA for flushing pixel data.
	* The sprite itself can be rendered later via <b>window.draw()</b>
	*/
	class SFMLRenderingTarget : public RenderingTarget {
		public:
			void setup(const Size& size);
			sf::RenderTexture& getRenderTexture();
			sf::Sprite& getSprite();

			float getRenderingScale() const;
			void setRenderingScale(const float value);

			void flush(const Rectangle& bounds, const std::span<uint8_t> pixelBuffer) override;

		protected:
			void onDefaultSizeChanged() override;
			void onRotationChanged() override;

		private:
			sf::RenderTexture _renderTexture {};
			sf::Sprite _sprite { _renderTexture.getTexture() };

			using RenderingTarget::setup;

			void updateTextureSize();
	};
}

#endif