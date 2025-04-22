#include "image.h"

namespace YOBA {
	Image::Image(const Size& size, const uint8_t* bitmap) :
		_size(size),
		_bitmap(bitmap)
	{

	}

	const Size& Image::getSize() const {
		return _size;
	}

	const uint8_t* Image::getBitmap() const {
		return _bitmap;
	}
}