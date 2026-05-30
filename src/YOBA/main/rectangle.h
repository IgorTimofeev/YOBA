#pragma once

#include <cstdint>
#include "vector2.h"
#include "size.h"

namespace YOBA {
	class Rectangle {
		public:
			constexpr Rectangle(const int32_t x, const int32_t y, const int32_t width, const int32_t height) :
				_x(x),
				_y(y),
				_width(width),
				_height(height)
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
			static const Rectangle invalidValue;

			int32_t getX() const {
				return _x;
			}

			void setX(const int32_t value) {
				_x = value;
			}

			int32_t getY() const {
				return _y;
			}

			void setY(const int32_t value) {
				_y = value;
			}

			Point getPosition() const {
				return {
					_x,
					_y
				};
			}

			void setPosition(const Point& value) {
				_x = value.getX();
				_y = value.getY();
			}

			uint16_t getWidth() const {
				return _width;
			}

			void setWidth(const uint16_t value) {
				_width = value;
			}

			uint16_t getHeight() const {
				return _height;
			}

			void setHeight(const uint16_t value) {
				_height = value;
			}

			Size getSize() const {
				return {
					_width,
					_height
				};
			}

			void setSize(const Size& value) {
				_width = value.getWidth();
				_height = value.getHeight();
			}

			int32_t getX2() const {
				return _x + _width - 1;
			}

			int32_t getY2() const {
				return _y + _height - 1;
			}

			int32_t getXCenter() const {
				return _x + _width / 2;
			}

			int32_t getYCenter() const {
				return _y + _height / 2;
			}

			Point getCenter() const {
				return {
					getXCenter(),
					getYCenter()
				};
			}

			uint32_t getSquare() const {
				return _width * _height;
			}

			Point getTopLeft() const {
				return getPosition();
			}

			Point getTopRight() const {
				return {
					getX2(),
					_y
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
					_x,
					getY2()
				};
			}

			bool intersects(const Rectangle& rectangle) const {
				return !(
					_x > rectangle.getX2()
					|| getX2() < rectangle._x
					|| _y > rectangle.getY2()
					|| getY2() < rectangle._y
				);
			}

			bool contains(const Point& point) const {
				return
					point.getX() >= _x
					&& point.getY() >= _y
					&& point.getX() <= getX2()
					&& point.getY() <= getY2();
			}

			bool contains(const Rectangle& rectangle) const {
				return
					rectangle.getX() >= _x
					&& rectangle.getY() >= _y
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

				result._x = std::max(_x, rectangle._x);
				result._y = std::max(_y, rectangle._y);
				result._width = std::min(_x + _width, rectangle._x + rectangle._width) - result._x;
				result._height = std::min(_y + _height, rectangle._y + rectangle._height) - result._y;

				return result;
			}

			Rectangle expandTo(const Rectangle& rectangle) const {
				Rectangle result {};

				result._x = std::min(_x, rectangle._x);
				result._x = std::min(_y, rectangle._y);
				result._width = std::max(_x + _width, rectangle._x + rectangle._width) - result._x;
				result._height = std::max(_y + _height, rectangle._y + rectangle._height) - result._y;

				return result;
			}

		private:
			int32_t _x = 0;
			int32_t _y = 0;
			uint16_t _width = 0;
			uint16_t _height = 0;
	};

	constexpr Rectangle Rectangle::invalidValue = { -1, -1, 0, 0 };

	// TODO: remove this in other projects
	using Bounds = Rectangle;
}