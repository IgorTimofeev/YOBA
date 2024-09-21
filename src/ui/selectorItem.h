#pragma once

#include "layout.h"
#include "vector"
#include "../event.h"

namespace yoba {
	class Selector;

	class SelectorItem : public Layout {
		public:
			bool isSelected() const {
				return _isSelected;
			}

			virtual void setSelected(bool value) {
				_isSelected = value;
			}

			Selector *getSelector() const;

			void setSelector(Selector *value);

		protected:
			void onEvent(InputEvent &event) override;

		private:
			Selector* _selector = nullptr;
			bool _isSelected = false;
	};
}