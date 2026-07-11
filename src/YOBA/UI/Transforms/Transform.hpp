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

			virtual Size processAvailableSizeForMeasure(Element* element, const Size& availableSize);
			virtual Size processMeasuredSize(Element* element, const Size& measuredSize);
			virtual Rectangle processLayoutBounds(Element* element, const Rectangle& layoutBounds);
			virtual Rectangle computeRenderingBounds(Element* element, const Rectangle& parentBounds);
	};
}