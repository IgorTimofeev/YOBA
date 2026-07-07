#pragma once

#include <YOBA/Core/Orientation.hpp>
#include <YOBA/UI/Element.hpp>

namespace YOBA {
	class OrientationElement : public virtual Element {
		public:
			Orientation getOrientation() const {
				return _orientation;
			}

			void setOrientation(const Orientation value) {
				if (value == _orientation)
					return;

				_orientation = value;

				invalidate();
			}

		private:
			Orientation _orientation = Orientation::vertical;
	};
}
