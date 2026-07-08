#include <YOBA/System.hpp>

#if defined(YOBA_SYSTEM_DESKTOP)

#pragma once

#include <cstdint>
#include <span>

#include <YOBA/Rendering/RenderingTarget.hpp>

#include <SFML/Graphics.hpp>

namespace YOBA {
	class SFMLRenderingTarget : public RenderingTarget {
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

			}

			sf::RenderWindow* getWindow() const {
				return _window;
			}

			float getRenderingScale() const {
				return _renderingScale;
			}

			void writePixels(const Rectangle& bounds, const std::span<uint8_t> pixelBuffer) override {

			}

		protected:
			
		private:
			sf::RenderWindow* _window = nullptr;
			float _renderingScale = 1;

			using RenderingTarget::setup;
	};
}

#endif