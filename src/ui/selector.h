#pragma once

#include "layout.h"
#include "vector"

namespace yoba {
	class SelectorItem;

	class Selector : public Layout {
		public:
			size_t getItemsCount();

			SelectorItem* getItemAt(size_t index);

			int32_t getIndexOfItem(SelectorItem* item);

			void addItem(SelectorItem* item);

			int32_t getSelectedIndex() const;

			void setSelectedIndex(int32_t index);

			void setItemsLayout(Layout* layout);

			const Callback<>& getOnSelectionChanged() const;

		protected:
			virtual void onSelectionChanged();

		private:
			Layout* _itemsLayout = nullptr;
			int32_t _selectedIndex = -1;
			Callback<> _onSelectionChanged {};
	};
}