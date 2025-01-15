#pragma once

#include "ui/element.h"

namespace yoba::ui {
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
