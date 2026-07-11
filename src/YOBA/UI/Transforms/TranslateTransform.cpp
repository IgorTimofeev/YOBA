#include <YOBA/UI/Transforms/TranslateTransform.hpp>

namespace YOBA {
	TranslateTransform::TranslateTransform(const Point& offset) : _offset(offset) {

	}

	TranslateTransform::TranslateTransform(): _offset(Point(0)) {

	}

	const Point& TranslateTransform::getOffset() const {
		return _offset;
	}

	void TranslateTransform::setOffset(const Point& value) {
		_offset = value;
	}

	Rectangle TranslateTransform::processLayoutBoundsOnArrange(const Element* element, const Rectangle& bounds) {
		return {
			bounds.getX() + _offset.getX(),
			bounds.getY() + _offset.getY(),
			bounds.getWidth(),
			bounds.getHeight()
		};
	}
}
