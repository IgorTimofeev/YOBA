#pragma once

#include <cstdint>
#include "YOBA/main/size.h"

namespace YOBA {
	class Image {
		public:
			Image(
				const Size& size,
				const uint8_t* bitmap
			);

			const Size& getSize() const;
			const uint8_t* getBitmap() const;

		private:
			const Size _size;
			const uint8_t* _bitmap;
	};
}