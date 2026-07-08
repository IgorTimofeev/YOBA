#include <YOBA/System.hpp>

#if defined(YOBA_SYSTEM_WINDOWS)

#pragma once

#include <YOBA/Rendering/Renderer.hpp>
#include <YOBA/Rendering/Desktop/SFMLRenderingTarget.hpp>

#include <SFML/Graphics.hpp>

namespace YOBA {
	class SFMLRenderer : public Renderer {
		public:
			void flush() override {
				//auto target = reinterpret_cast<SFMLRenderingTarget*>(getTarget());

				//target->getWindow()->display();
			}

		protected:
			void clearNative(const Color* color) override {
				auto target = reinterpret_cast<SFMLRenderingTarget*>(getTarget());
				
				SFDrawRectangle(Bounds(target->getSize()), color);
			}

			void renderPixelNative(const Point& point, const Color* color) override {
				SFDrawRectangle(Bounds(point.getX(), point.getY(), 1, 1), color);
			}

			void renderHorizontalLineNative(const Point& point, uint16_t length, const Color* color) override {
				SFDrawRectangle(Bounds(point.getX(), point.getY(), length, 1), color);
			}
			
			void renderVerticalLineNative(const Point& point, uint16_t length, const Color* color) override {
				SFDrawRectangle(Bounds(point.getX(), point.getY(), 1, length), color);
			}

			void renderFilledRectangleNative(const Rectangle& bounds, const Color* color) override {
				SFDrawRectangle(bounds, color);
			}

			void renderImageNative(const Point& point, const Image* image) override {
				auto target = reinterpret_cast<SFMLRenderingTarget*>(getTarget());
				
			}

		private:
			static sf::Color YOBAColorToSFColor(const Color* color) {
				auto color888 = reinterpret_cast<const RGB888Color*>(color);

				return {
					color888->getR(),
					color888->getG(),
					color888->getB()
				};
			}

			void SFDrawRectangle(const Bounds& bounds, const Color* color) {
				auto target = reinterpret_cast<SFMLRenderingTarget*>(getTarget());
				const auto SFColor = YOBAColorToSFColor(color);

				sf::Vertex vertices[] {
					sf::Vertex(
						sf::Vector2f(
							bounds.getX() * target->getRenderingScale(),
							bounds.getY() * target->getRenderingScale()
						),
						SFColor
					),
					sf::Vertex(
						sf::Vector2f(
							(bounds.getX2() + 1) * target->getRenderingScale(),
							bounds.getY() * target->getRenderingScale()
						),
						SFColor
					),
					sf::Vertex(
						sf::Vector2f(
							bounds.getX() * target->getRenderingScale(),
							(bounds.getY2() + 1) * target->getRenderingScale()
						),
						SFColor
					),
					sf::Vertex(
						sf::Vector2f(
							(bounds.getX2() + 1) * target->getRenderingScale(),
							(bounds.getY2() + 1) * target->getRenderingScale()
						),
						SFColor
					)
				};

				target->getWindow()->draw(vertices, std::size(vertices), sf::PrimitiveType::TriangleStrip);
			}
	};
}

#endif