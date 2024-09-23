#pragma once

#include <cstdint>
#include "size.h"

namespace yoba {
	class Image {
		public:
			Image(
				const Size& size,
				const uint8_t* bitmap
			) :
				_size(size),
				_bitmap(bitmap)
			{

			}

			const Size& getSize() const {
				return _size;
			}

			const uint8_t* getBitmap() const {
				return _bitmap;
			}

		private:
			const Size _size;
			const uint8_t* _bitmap;
	};
}