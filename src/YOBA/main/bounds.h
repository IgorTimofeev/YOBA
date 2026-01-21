#pragma once

#include <cstdint>
#include "vector2.h"
#include "size.h"

namespace YOBA {
	class Bounds {
		public:
			constexpr Bounds(const int32_t x, const int32_t y, const int32_t width, const int32_t height) :
				x(x),
				y(y),
				width(width),
				height(height)
			{

			}

			constexpr Bounds() : Bounds(0, 0, 0, 0) {

			}

			constexpr Bounds(const Point& position, const Size& size) : Bounds(
				position.getX(),
				position.getY(),
				size.getWidth(),
				size.getHeight()
			) {

			}

			constexpr Bounds(const Size& size) : Bounds(Point(), size) {

			}

			// Defines a element bounds that couldn't exist in any UI
			static const Bounds& invalidValue;

			int32_t getX() const {
				return x;
			}

			void setX(const int32_t value) {
				x = value;
			}

			int32_t getY() const {
				return y;
			}

			void setY(const int32_t value) {
				y = value;
			}

			Point getPosition() const {
				return {
					x,
					y
				};
			}

			void setPosition(const Point& value) {
				x = value.getX();
				y = value.getY();
			}

			uint16_t getWidth() const {
				return width;
			}

			void setWidth(const uint16_t value) {
				width = value;
			}

			uint16_t getHeight() const {
				return height;
			}

			void setHeight(const uint16_t value) {
				height = value;
			}

			Size getSize() const {
				return {
					width,
					height
				};
			}

			void setSize(const Size& value) {
				width = value.getWidth();
				height = value.getHeight();
			}

			int32_t getX2() const {
				return x + width - 1;
			}

			int32_t getY2() const {
				return y + height - 1;
			}

			int32_t getXCenter() const {
				return x + width / 2;
			}

			int32_t getYCenter() const {
				return y + height / 2;
			}

			Point getCenter() const {
				return {
					getXCenter(),
					getYCenter()
				};
			}

			uint32_t getSquare() const {
				return width * height;
			}

			Point getTopLeft() const {
				return getPosition();
			}

			Point getTopRight() const {
				return {
					getX2(),
					y
				};
			}

			Point getBottomRight() const {
				return {
					getX2(),
					getY2()
				};
			}

			Point getBottomLeft() const {
				return {
					x,
					getY2()
				};
			}

			bool intersects(const Bounds& bounds) const {
				return !(
					x > bounds.getX2()
					|| getX2() < bounds.x
					|| y > bounds.getY2()
					|| getY2() < bounds.y
				);
			}

			bool contains(const Point& point) const {
				return
					point.getX() >= x
					&& point.getY() >= y
					&& point.getX() <= getX2()
					&& point.getY() <= getY2();
			}

			bool contains(const Bounds& bounds) const {
				return
					bounds.getX() >= x
					&& bounds.getY() >= y
					&& bounds.getX2() <= getX2()
					&& bounds.getY2() <= getY2();
			}

			bool haveZeroSize() const {
				return getWidth() == 0 || getHeight() == 0;
			}

			bool isLandscape() const {
				return getWidth() > getHeight();
			}

			Bounds getIntersection(const Bounds& bounds) const {
				Bounds result {};

				result.x = std::max(x, bounds.x);
				result.y = std::max(y, bounds.y);
				result.width = std::min(x + width, bounds.x + bounds.width) - result.x;
				result.height = std::min(y + height, bounds.y + bounds.height) - result.y;

				return result;
			}

			Bounds getExpansion(const Bounds& bounds) const {
				Bounds result {};

				result.x = std::min(x, bounds.x);
				result.x = std::min(y, bounds.y);
				result.width = std::max(x + width, bounds.x + bounds.width) - result.x;
				result.height = std::max(y + height, bounds.y + bounds.height) - result.y;

				return result;
			}

		private:
			int32_t x = 0;
			int32_t y = 0;
			uint16_t width = 0;
			uint16_t height = 0;
	};
}