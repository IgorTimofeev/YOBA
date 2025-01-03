#pragma once

#include "element.h"

namespace yoba {
	class FocusableElement : public virtual Element {
		public:
			~FocusableElement();

			bool isFocusable() const;
			void setFocusable(bool focusable);

			bool isFocused();
			void setFocused(bool value);

		protected:
			bool _focusable = true;
	};
}
