#pragma once

#include <cstdint>
#include <span>

#include <YOBA/UI/Transforms/Transform.hpp>

namespace YOBA {
	// Allows to apply multiple transforms at once
	class TransformGroup : public Transform {
		public:
			TransformGroup(const std::span<Transform*> transforms);
			TransformGroup();

			Size processAvailableSizeForMeasure(Element* element, const Size& availableSize) override;
			Size processMeasuredSize(Element* element, const Size& measuredSize) override;
			Rectangle processLayoutBounds(Element* element, const Rectangle& layoutBounds) override;

		private:
			std::span<Transform*> _transforms;
	};
}