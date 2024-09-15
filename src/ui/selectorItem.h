#pragma once

#include "layout.h"
#include "vector"

namespace yoba {
	class Selector;

	class SelectorItem : public Layout {
		public:
			virtual void setSelected(const bool& value);

			Selector *getSelector() const;

			void setSelector(Selector *value);

		protected:
			void onEvent(Event &event) override;

		private:
			Selector* _selector = nullptr;
	};
}