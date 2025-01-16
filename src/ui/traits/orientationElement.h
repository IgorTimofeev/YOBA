#pragma once

#include "ui/element.h"

namespace yoba::ui {
	class OrientationElement : public virtual Element {
		public:
			Orientation getOrientation() const {
				return _orientation;
			}

			void setOrientation(Orientation orientation) {
				_orientation = orientation;
			}

		private:
			Orientation _orientation = Orientation::vertical;
	};
}
