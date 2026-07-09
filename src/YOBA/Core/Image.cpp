#include <YOBA/Core/Image.hpp>

namespace YOBA {
	uint8_t Image::getOptions() const {
		return _options;
	}

	const Size& Image::getSize() const {
		return _size;
	}

	const uint8_t* Image::getBitmap() const {
		return _bitmap;
	}
}