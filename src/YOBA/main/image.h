#pragma once

#include <cstdint>
#include "YOBA/main/size.h"

namespace YOBA {
	namespace ImageFlags {
		enum : uint8_t {
			none = 0b0000'0000,
			palette8 = 0b0000'0001,
			alpha = 0b0000'0010,
		};
	}

	class Image {
		public:
			Image(
				const Size& size,
				const uint8_t* bitmap,
				uint8_t flags = ImageFlags::none
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