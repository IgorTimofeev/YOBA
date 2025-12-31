#pragma once

#include <cstdint>
#include <YOBA/main/size.h>

namespace YOBA {
	// I love C++
	namespace ImageFlags {
		enum : uint8_t {
			RGB565 = 0b0000'0001,
			palette8Bit = 0b0000'0010,
			alpha1Bit = 0b0000'0100,
		};
	}

	class Image {
		public:
			constexpr Image(
				const uint8_t flags,
				const Size& size,
				const uint8_t* bitmap
			) :
				flags(flags),
				size(size),
				bitmap(bitmap)
			{
			
			}

			uint8_t getFlags() const;
			const Size& getSize() const;
			const uint8_t* getBitmap() const;

		private:
			const uint8_t flags;
			const Size size;
			const uint8_t* bitmap;
	};
}