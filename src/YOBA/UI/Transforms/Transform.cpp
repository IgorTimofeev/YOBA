#include <YOBA/UI/Transforms/Transform.hpp>

namespace YOBA {
	Size Transform::processAvailableSizeBeforeMeasure(const Element* element, const Size& availableSize) {
		return availableSize;
	}

	Size Transform::processMeasuredSizeAfterMeasure(const Element* element, const Size& measuredSize) {
		return measuredSize;
	}

	Rectangle Transform::processLayoutBoundsOnArrange(const Element* element, const Rectangle& bounds) {
		return bounds;
	}

	Rectangle Transform::processRenderingBoundsOnArrange(const Element* element, const Rectangle& bounds) {
		return processLayoutBoundsOnArrange(element, bounds);
	}
}