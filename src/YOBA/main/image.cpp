#include "image.h"

namespace YOBA {
	Image::Image(
		const Size& size,
		const uint8_t* bitmap,
		uint8_t flags
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