#include <YOBA/Core/Image.hpp>

namespace YOBA {
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