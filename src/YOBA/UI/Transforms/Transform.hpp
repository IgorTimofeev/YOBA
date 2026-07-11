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

			virtual Size processAvailableSizeBeforeMeasure(const Element* element, const Size& availableSize);
			virtual Size processMeasuredSizeAfterMeasure(const Element* element, const Size& measuredSize);

			virtual Rectangle processLayoutBoundsOnArrange(const Element* element, const Rectangle& bounds);
			virtual Rectangle processRenderingBoundsOnArrange(const Element* element, const Rectangle& bounds);
	};
}