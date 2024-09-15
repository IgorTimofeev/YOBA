#pragma once

#include <cstdint>
#include "size.h"

namespace yoba {
	class ImageSource {
		public:
			explicit ImageSource(const Size &size) : _size(size) {

			}

			ImageSource(const Size &size, uint16_t *pixels) : _size(size), _pixels(pixels) {

			}

			ImageSource(const Size &size, uint16_t& pixels) : _size(size), _pixels(&pixels) {

			}

			const Size &getSize() const {
				return _size;
			}

			void setSize(const Size &size) {
				_size = size;
			}

			uint16_t *getPixels() const {
				return _pixels;
			}

			void setPixels(uint16_t *pixels) {
				_pixels = pixels;
			}

		private:
			Size _size;
			uint16_t* _pixels = nullptr;
	};
}