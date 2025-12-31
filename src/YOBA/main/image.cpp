#include "image.h"

namespace YOBA {


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