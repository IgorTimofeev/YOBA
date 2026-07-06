#pragma once

#include <cstdint>

#include <YOBA/core/vector2.h>

namespace YOBA {
	class Size {
		public:
			constexpr Size(const uint16_t width, const uint16_t height) :
				_width(width),
				_height(height)
			{
			
			}
			
			constexpr Size(const uint16_t uniformValue) : Size(uniformValue, uniformValue) {

			}
			
			constexpr Size() : Size(0, 0) {
			
			}

			// Represents size value that should be interpreted by library as "automatically computed" during
			// layout processing. At this moment, there's actually 2 situations when you should use it:
			//
			// 1) When setting element size explicitly by one axis, but wanting to keep automatic calculations by another.
			//    For example, if you call button.setSize({ 100, Size::computed }), the library will keep button
			//    width at 100 pixels, but will continue to adjust its height based on its contents
			//
			// 2) When developing custom layouts, Size::computed represents an infinite value during
			//    measure/arrange pass. For example, when measuring vertical StackLayout I'm telling library that
			//    there's no restrictions by vertical axis via child.onMeasure({ availableSize.getWidth(), Size::computed })
			constexpr static uint16_t computed = 0xFFFF;

			uint16_t getWidth() const;
			void setWidth(uint16_t value);

			uint16_t getHeight() const;
			void setHeight(uint16_t value);

			uint16_t getXCenter() const;
			uint16_t getYCenter() const;
			Point getCenter() const;
			uint32_t getSquare() const;

			Size interpolate(const Size& to, const float& position) const;

			bool operator==(const Size& rhs) const;
			bool operator!=(const Size& rhs) const;

		private:
			uint16_t _width;
			uint16_t _height;
	};
}