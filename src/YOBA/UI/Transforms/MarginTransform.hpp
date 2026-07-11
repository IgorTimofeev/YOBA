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

			Rectangle computeLayoutBoundsOnArrange(Element* element, const Rectangle& parentBounds) override;
			Size computeAvailableSizeBeforeMeasure(Element* element, const Size& availableSize) override;
			Size computeMeasuredSizeAfterMeasure(Element* element, const Size& measuredSize) override;

		private:
			Margin _margin;
	};
}