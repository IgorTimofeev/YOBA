#pragma once

#include "../element.h"

namespace yoba::ui {
	class SelectableElement : public virtual Element {
		public:
			bool isSelected() const {
				return _selected;
			}

			void setSelected(bool value) {
				if (value == _selected)
					return;

				_selected = value;

				onSelectionChanged();

				invalidate();
			}

		protected:
			virtual void onSelectionChanged() {

			}

		private:
			bool _selected = false;

	};
}
