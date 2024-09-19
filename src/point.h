#pragma once

#include "Arduino.h"
#include <cstdint>

namespace yoba {
	class Point {
		public:
			Point(const Point& point) = default;

			Point() : _x(0), _y(0) {

			}

			Point(int32_t x, int32_t y) : _x(x), _y(y) {

			}

			int32_t getX() const {
				return _x;
			}

			void setX(int32_t value) {
				_x = value;
			}

			int32_t getY() const {
				return _y;
			}

			void setY(int32_t value) {
				_y = value;
			}

			Point rotate(float angleInRadians) const {
				auto angleSin = sin(angleInRadians);
				auto angleCos = cos(angleInRadians);

				return {
					(int32_t) ((float) _x * angleCos - (float) _y * angleSin),
					(int32_t) ((float) _x * angleSin - (float) _y * angleCos)
				};
			}

			float getRotation(const Point& point) const {
				return atan2f((float) (point._y - _y), (float) (point._x - _x));
			}

			float getDistance() const {
				return sqrt((float) _x * (float) _x + (float) _y * (float) _y);
			}

			Point perpendicular() const {
				return {
					-_y,
					_x,
				};
			}

			Point operator+(const Point& right) const {
				return {
					_x + right._x,
					_y + right._y,
				};
			}

			Point operator-(const Point& right) const {
				return {
					_x - right._x,
					_y - right._y,
				};
			}

			Point operator*(float value) const {
				return {
					(int32_t) ((float) _x * value),
					(int32_t) ((float) _y * value),
				};
			}

			bool operator==(const Point &right) const {
				return _x == right._x && _y == right._y;
			}

			bool operator!=(const Point &right) const {
				return !operator==(right);
			}

		private:
			int32_t _x;
			int32_t _y;
	};
}