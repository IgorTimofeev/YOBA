#pragma once

#include <cstdint>
#include "vector2.h"
#include "size.h"

namespace YOBA {
	class Rectangle {
		public:
			constexpr Rectangle(const int32_t x, const int32_t y, const int32_t width, const int32_t height) :
				x(x),
				y(y),
				width(width),
				height(height)
			{

			}

			constexpr Rectangle() : Rectangle(0, 0, 0, 0) {

			}

			constexpr Rectangle(const Point& position, const Size& size) : Rectangle(
				position.getX(),
				position.getY(),
				size.getWidth(),
				size.getHeight()
			) {

			}

			constexpr Rectangle(const Size& size) : Rectangle(Point(), size) {

			}

			// Defines an element rectangle that couldn't exist in any UI
			constexpr static Rectangle invalidValue = { -1, -1, 0, 0 };

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

			bool intersects(const Rectangle& rectangle) const {
				return !(
					x > rectangle.getX2()
					|| getX2() < rectangle.x
					|| y > rectangle.getY2()
					|| getY2() < rectangle.y
				);
			}

			bool contains(const Point& point) const {
				return
					point.getX() >= x
					&& point.getY() >= y
					&& point.getX() <= getX2()
					&& point.getY() <= getY2();
			}

			bool contains(const Rectangle& rectangle) const {
				return
					rectangle.getX() >= x
					&& rectangle.getY() >= y
					&& rectangle.getX2() <= getX2()
					&& rectangle.getY2() <= getY2();
			}

			bool haveZeroSize() const {
				return getWidth() == 0 || getHeight() == 0;
			}

			bool isLandscape() const {
				return getWidth() > getHeight();
			}

			Rectangle getIntersection(const Rectangle& rectangle) const {
				Rectangle result {};

				result.x = std::max(x, rectangle.x);
				result.y = std::max(y, rectangle.y);
				result.width = std::min(x + width, rectangle.x + rectangle.width) - result.x;
				result.height = std::min(y + height, rectangle.y + rectangle.height) - result.y;

				return result;
			}

			Rectangle expandTo(const Rectangle& rectangle) const {
				Rectangle result {};

				result.x = std::min(x, rectangle.x);
				result.x = std::min(y, rectangle.y);
				result.width = std::max(x + width, rectangle.x + rectangle.width) - result.x;
				result.height = std::max(y + height, rectangle.y + rectangle.height) - result.y;

				return result;
			}

		private:
			int32_t x = 0;
			int32_t y = 0;
			uint16_t width = 0;
			uint16_t height = 0;
	};

	using Bounds = Rectangle;
}