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

		protected:
			void clearNative(const Color* color) override;
			void fillRectangleNative(const Rectangle& bounds, const Color* color) override;
			void putImageNative(const Rectangle& bounds, const Image* image) override;
			void putPixelNative(const Point& position, const Color* color) override;
			void strokeHorizontalLineNative(const Point& position, const uint16_t length, const Color* color) override;
			void strokeVerticalLineNative(const Point& position, const uint16_t length, const Color* color) override;

		private:

	};
}

#endif