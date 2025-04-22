#pragma once

#include <cstdint>
#include "vector2.h"
#include "size.h"

namespace YOBA {
	class Bounds {
		public:
			Bounds(int32_t x, int32_t y, int32_t width, int32_t height);
			Bounds();
			Bounds(const Point& position, const Size& size);
			explicit Bounds(const Size& size);

			int32_t getX() const;
			void setX(int32_t value);

			int32_t getY() const;
			void setY(int32_t value);

			Point getPosition() const;
			void setPosition(const Point& value);

			uint16_t getWidth() const;
			void setWidth(uint16_t value);

			uint16_t getHeight() const;
			void setHeight(uint16_t value);

			Size getSize() const;
			void setSize(const Size& value);


			int32_t getX2() const;
			int32_t getY2() const;
			int32_t getXCenter() const;
			int32_t getYCenter() const;
			uint32_t getSquare() const;

			Point getCenter() const;
			Point getTopLeft() const;
			Point getTopRight() const;
			Point getBottomRight() const;
			Point getBottomLeft() const;

			bool intersects(const Point& point) const;
			bool intersects(const Bounds& bounds) const;

			bool haveZeroSize() const;

			Bounds getIntersection(const Bounds& bounds) const;

			Bounds getExpansion(const Bounds& bounds) const;

		private:
			int32_t _x = 0;
			int32_t _y = 0;
			uint16_t _width = 0;
			uint16_t _height = 0;
	};
}