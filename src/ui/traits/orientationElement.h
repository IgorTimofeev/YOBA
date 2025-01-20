#pragma once

#include "ui/element.h"

namespace yoba::ui {
	class OrientationElement : public virtual Element {
		public:
			Orientation getOrientation() const {
				return _orientation;
			}

			void setOrientation(Orientation value) {
				if (value == _orientation)
					return;

				_orientation = value;

				invalidate();
			}

		private:
			Orientation _orientation = Orientation::vertical;
	};
}
