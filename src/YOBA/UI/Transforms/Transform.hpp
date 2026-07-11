#pragma once

#include <cstdint>
#include <span>

#include <YOBA/UI/Element.hpp>
#include <YOBA/Core/Size.hpp>
#include <YOBA/Core/Rectangle.hpp>

namespace YOBA {
	class Transform {
		public:
			virtual ~Transform() = default;

			virtual Size computeAvailableSizeBeforeMeasure(Element* element, const Size& availableSize) {
				return availableSize;
			}

			virtual Size computeMeasuredSizeAfterMeasure(Element* element, const Size& measuredSize) {
				return measuredSize;
			}

			virtual Rectangle computeLayoutBoundsOnArrange(Element* element, const Rectangle& parentBounds) {
				return element->defaultComputeLayoutBoundsOnArrange(parentBounds);
			}

			virtual Rectangle computeRenderingBounds(Element* element, const Rectangle& parentBounds);
	};
}