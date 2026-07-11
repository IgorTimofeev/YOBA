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

	Rectangle TranslateTransform::computeLayoutBoundsOnArrange(Element* element, const Rectangle& parentBounds) {
		return {
			parentBounds.getX() + _offset.getX(),
			parentBounds.getY() + _offset.getY(),
			parentBounds.getWidth(),
			parentBounds.getHeight()
		};
	}
}
