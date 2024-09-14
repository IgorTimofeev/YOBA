#pragma once

#include "element.h"
#include "color.h"

namespace yoba {
	class BackgroundAware : public virtual Element {
		public:
			const Color* getBackground() const {
				return _background;
			}

			void setBackground(const Color* value) {
				_background = value;
			}

		private:
			const Color* _background = &Color16::black;
	};
}
