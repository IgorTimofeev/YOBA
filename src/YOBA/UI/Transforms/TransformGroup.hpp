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

			Size processAvailableSizeBeforeMeasure(const Element* element, const Size& availableSize) override;
			Size processMeasuredSizeAfterMeasure(const Element* element, const Size& measuredSize) override;
			Rectangle processLayoutBoundsOnArrange(const Element* element, const Rectangle& bounds) override;
			Rectangle processRenderingBoundsOnArrange(const Element* element, const Rectangle& bounds) override;

		private:
			std::span<Transform*> _transforms;
	};
}