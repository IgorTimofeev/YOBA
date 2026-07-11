#include <YOBA/UI/Transforms/TransformGroup.hpp>

namespace YOBA {
	TransformGroup::TransformGroup(const std::span<Transform*> transforms): _transforms(transforms) {

	}

	TransformGroup::TransformGroup(): _transforms({}) {

	}

	Size TransformGroup::processAvailableSizeBeforeMeasure(const Element* element, const Size& availableSize) {
		if (_transforms.empty())
			return availableSize;

		Size result = availableSize;

		for (const auto transform : _transforms)
			result = transform->processAvailableSizeBeforeMeasure(element, result);

		return result;
	}

	Size TransformGroup::processMeasuredSizeAfterMeasure(const Element* element, const Size& measuredSize) {
		if (_transforms.empty())
			return measuredSize;

		Size result = measuredSize;

		for (const auto transform : _transforms)
			result = transform->processMeasuredSizeAfterMeasure(element, result);

		return result;
	}

	Rectangle TransformGroup::processLayoutBoundsOnArrange(const Element* element, const Rectangle& bounds) {
		if (_transforms.empty())
			return bounds;

		Bounds result = bounds;

		for (const auto transform : _transforms)
			result = transform->processLayoutBoundsOnArrange(element, result);

		return result;
	}

	Rectangle TransformGroup::processRenderingBoundsOnArrange(const Element* element, const Rectangle& bounds) {
		if (_transforms.empty())
			return bounds;

		Bounds result = bounds;

		for (const auto transform : _transforms)
			result = transform->processRenderingBoundsOnArrange(element, result);

		return result;
	}
}
