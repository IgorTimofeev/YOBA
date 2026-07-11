#include <YOBA/UI/Transforms/Transform.hpp>

namespace YOBA {
	Size Transform::processAvailableSizeForMeasure(Element* element, const Size& availableSize) {
		return availableSize;
	}

	Size Transform::processMeasuredSize(Element* element, const Size& measuredSize) {
		return measuredSize;
	}

	Rectangle Transform::processLayoutBounds(Element* element, const Rectangle& layoutBounds) {
		return layoutBounds;
	}

	Rectangle Transform::computeRenderingBounds(Element* element, const Rectangle& parentBounds) {
		return processLayoutBounds(element, parentBounds);
	}
}