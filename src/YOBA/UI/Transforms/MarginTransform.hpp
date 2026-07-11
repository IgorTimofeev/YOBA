#pragma once

#include <cstdint>

#include <YOBA/Core/Margin.hpp>
#include <YOBA/UI/Transforms/Transform.hpp>

namespace YOBA {
	class MarginTransform : public Transform {
		public:
			MarginTransform(const Margin& value);
			MarginTransform();

			const Margin& getMargin() const;
			void setMargin(const Margin& value);

			Size processAvailableSizeBeforeMeasure(const Element* element, const Size& availableSize) override;
			Size processMeasuredSizeAfterMeasure(const Element* element, const Size& measuredSize) override;
			Rectangle processLayoutBoundsOnArrange(const Element* element, const Rectangle& bounds) override;

		private:
			Margin _margin;
	};
}