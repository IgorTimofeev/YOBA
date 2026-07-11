#include <YOBA/UI/Transforms/TransformGroup.hpp>

namespace YOBA {
	TransformGroup::TransformGroup(const std::span<Transform*> transforms): _transforms(transforms) {

	}

	TransformGroup::TransformGroup(): _transforms({}) {

	}

	Size TransformGroup::processAvailableSizeForMeasure(Element* element, const Size& availableSize) {
		if (_transforms.empty())
			return availableSize;

		Size result = availableSize;

		for (const auto transform : _transforms)
			result = transform->processAvailableSizeForMeasure(element, result);

		return result;
	}

	Size TransformGroup::processMeasuredSize(Element* element, const Size& measuredSize) {
		if (_transforms.empty())
			return measuredSize;

		Size result = measuredSize;

		for (const auto transform : _transforms)
			result = transform->processMeasuredSize(element, result);

		return result;
	}

	Rectangle TransformGroup::processLayoutBounds(Element* element, const Rectangle& layoutBounds) {
		if (_transforms.empty())
			return layoutBounds;

		Rectangle result = layoutBounds;

		for (const auto transform : _transforms)
			result = transform->processLayoutBounds(element, result);

		return result;
	}

}
