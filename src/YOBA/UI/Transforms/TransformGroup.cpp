#include <YOBA/UI/Transforms/TransformGroup.hpp>

namespace YOBA {
	TransformGroup::TransformGroup(const std::span<Transform*> transforms): _transforms(transforms) {

	}

	TransformGroup::TransformGroup(): _transforms({}) {

	}

	// Size TransformGroup::processAvailableSizeBeforeMeasure(const Element* element, const Size& availableSize) {
	// 	if (_transforms.empty())
	// 		return availableSize;
	//
	// 	Size result = availableSize;
	//
	// 	for (const auto transform : _transforms)
	// 		result = transform->processAvailableSizeBeforeMeasure(element, result);
	//
	// 	return result;
	// }

}
