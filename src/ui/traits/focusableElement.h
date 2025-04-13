#pragma once

#include "ui/element.h"

namespace yoba::ui {
	class FocusableElement : public virtual Element {
		public:
			bool isFocusable() const;
			void setFocusable(bool focusable);

			bool isFocused();
			void setFocused(bool value);

		protected:
			void setApplication(Application* value) override;

		private:
			bool _focusable = true;
	};
}
