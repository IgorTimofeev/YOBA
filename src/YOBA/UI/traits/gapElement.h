#pragma once

#include <YOBA/UI/element.h>

namespace YOBA {
	class GapElement : public virtual Element {
		public:
			int32_t getGap() const {
				return _gap;
			}

			void setGap(const int32_t value) {
				if (value == _gap)
					return;

				_gap = value;

				invalidate();
			}

		private:
			int32_t _gap = 0;
	};
}
