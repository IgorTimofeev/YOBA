#pragma once

#include "ui/element.h"
#include "color.h"

namespace yoba {
	class ForegroundAware : public virtual Element {
		public:
			const Color* getForeground() const {
				return _foreground;
			}

			void setForeground(const Color* value) {
				_foreground = value;

				invalidate();
			}

		private:
			const Color* _foreground = &Color16::white;
	};
}
