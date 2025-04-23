#pragma once

#include <cstdint>
#include "YOBA/main/size.h"

namespace YOBA {
	// I love C++
	namespace ImageFlags {
		enum : uint8_t {
			RGB565 = 0b0000'0000,
			palette8Bit = 0b0000'0001,
			alpha1Bit = 0b0000'0010,
		};
	}

	class Image {
		public:
			Image(
				uint8_t flags,
				const Size& size,
				const uint8_t* bitmap
			);

			uint8_t getFlags() const;
			const Size& getSize() const;
			const uint8_t* getBitmap() const;

		private:
			const uint8_t _flags;
			const Size _size;
			const uint8_t* _bitmap;
	};
}