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

			Size processAvailableSizeForMeasure(Element* element, const Size& availableSize) override;
			Size processMeasuredSize(Element* element, const Size& measuredSize) override;
			Rectangle processLayoutBounds(Element* element, const Rectangle& layoutBounds) override;

		private:
			Margin _margin;
	};
}