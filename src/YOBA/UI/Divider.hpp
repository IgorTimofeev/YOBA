#pragma once

#include <YOBA/UI/Control.hpp>
#include <YOBA/UI/Traits/FillColorElement.hpp>
#include <YOBA/UI/Traits/OrientationElement.hpp>

namespace YOBA {
	class Divider : public Control, public FillColorElement, public OrientationElement {
		public:
			Divider(const Orientation orientation);
			Divider(const Orientation orientation, const Color* color);
			Divider(const Color* color);
			Divider();

			uint8_t getLineThickness() const;
			void setLineThickness(const uint8_t value);

		protected:
			Size onMeasure(const Size& availableSize) override;
			void onRender(Renderer* renderer, const Rectangle& bounds) override;

		private:
			uint8_t _lineThickness = 1;
	};
}
