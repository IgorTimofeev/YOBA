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

	Rectangle TranslateTransform::processLayoutBounds(Element* element, const Rectangle& layoutBounds) {
		return {
			layoutBounds.getX() + _offset.getX(),
			layoutBounds.getY() + _offset.getY(),
			layoutBounds.getWidth(),
			layoutBounds.getHeight()
		};
	}
}
