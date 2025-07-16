#include "image.h"

namespace YOBA {
	Image::Image(
		const uint8_t flags,
		const Size& size,
		const uint8_t* bitmap
	) :
		flags(flags),
		size(size),
		bitmap(bitmap)
	{

	}

	uint8_t Image::getFlags() const {
		return flags;
	}

	const Size& Image::getSize() const {
		return size;
	}

	const uint8_t* Image::getBitmap() const {
		return bitmap;
	}
}