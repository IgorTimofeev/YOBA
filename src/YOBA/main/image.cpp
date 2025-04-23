#include "image.h"

namespace YOBA {
	Image::Image(
		uint8_t flags,
		const Size& size,
		const uint8_t* bitmap
	) :
		_flags(flags),
		_size(size),
		_bitmap(bitmap)
	{

	}

	uint8_t Image::getFlags() const {
		return _flags;
	}

	const Size& Image::getSize() const {
		return _size;
	}

	const uint8_t* Image::getBitmap() const {
		return _bitmap;
	}
}