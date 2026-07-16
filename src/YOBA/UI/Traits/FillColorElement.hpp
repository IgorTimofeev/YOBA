#pragma once

#include <YOBA/UI/Element.hpp>
#include <YOBA/Core/Colors.hpp>

namespace YOBA {
	class FillColorElement : public virtual Element {
		public:
			const Color* getFillColor() const {
				return _fillColor;
			}

			void setFillColor(const Color* value) {
				if (value == _fillColor)
					return;

				_fillColor = value;

				invalidate();
			}

		private:
			const Color* _fillColor = nullptr;
	};
}
