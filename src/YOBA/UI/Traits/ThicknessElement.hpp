#pragma once

#include <YOBA/UI/Element.hpp>

namespace YOBA {
	class ThicknessElement : public virtual Element {
		public:
			uint8_t getThickness() const {
				return _thickness;
			}

			void setThickness(const uint8_t value) {
				if (value == _thickness)
					return;

				_thickness= value;

				invalidate();
			}

		private:
			uint8_t _thickness = 1;
	};
}
