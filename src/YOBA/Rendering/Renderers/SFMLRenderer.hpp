#pragma once

#include <YOBA/System.hpp>

#ifdef YOBA_SYSTEM_SFML

#include <YOBA/Core.hpp>
#include <YOBA/Rendering/Renderers/Renderer.hpp>
#include <YOBA/Rendering/Targets/SFMLRenderingTarget.hpp>

namespace YOBA {
	class SFMLRenderer : public virtual Renderer {
		public:
			void flush() override;
			void putSprite(const Point& position, sf::Sprite* sprite) const;

		protected:
			void fillRectangleNative(const Rectangle& bounds, const Color* color) override;
			void putImageNative(const Point& position, const Image* image) override;

		private:

	};
}

#endif
