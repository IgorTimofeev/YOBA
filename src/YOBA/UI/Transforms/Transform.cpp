#include <YOBA/UI/Transforms/Transform.hpp>

namespace YOBA {
	Rectangle Transform::computeRenderingBounds(Element* element, const Rectangle& parentBounds) {
		return computeLayoutBoundsOnArrange(element, parentBounds);
	}
}