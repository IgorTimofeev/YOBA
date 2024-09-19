#pragma once

#include "Arduino.h"
#include <cstdint>
#include <climits>
#include <limits>

namespace yoba {
	class Size {
		public:
			static const uint16_t Calculated = 0xFFFF;

			Size() :
				_width(0),
				_height(0)
			{

			}

			Size(uint16_t width, uint16_t height) :
				_width(width),
				_height(height)
			{

			}

			Size(const Size& size) = default;

			uint16_t getWidth() const {
				return _width;
			}

			void setWidth(uint16_t value) {
				_width = value;
			}

			uint16_t getHeight() const {
				return _height;
			}

			void setHeight(uint16_t value) {
				_height = value;
			}

			Size interpolate(const Size &to, const double &position) const {
				return {
					(uint16_t) (getWidth() + position * (to.getWidth() - getWidth())),
					(uint16_t) (getHeight() + position * (to.getHeight() - getHeight()))
				};
			}

		private:
			uint16_t _width;
			uint16_t _height;
	};
}