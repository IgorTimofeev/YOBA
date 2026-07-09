#pragma once

#include <cstdint>

#include <YOBA/Core/Size.hpp>

namespace YOBA {
	// I love C++
	namespace ImageOptions {
		enum : uint8_t {
			monochrome =  0b00000001,
			RGB565 =      0b00000010,
			RGB666 =      0b00000100,
			RGB888 =      0b00001000,
			palette8Bit = 0b00010000,
			alpha1Bit =   0b00100100,
		};
	}

	class Image {
		public:
			constexpr Image(
				const uint8_t options,
				const Size& size,
				const uint8_t* bitmap
			) :
				_options(options),
				_size(size),
				_bitmap(bitmap)
			{
			
			}

			uint8_t getOptions() const;
			const Size& getSize() const;
			const uint8_t* getBitmap() const;

		private:
			const uint8_t _options;
			const Size _size;
			const uint8_t* _bitmap;
	};
}