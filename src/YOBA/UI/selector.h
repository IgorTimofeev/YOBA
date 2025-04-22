#pragma once

#include "layout.h"
#include "vector"
#include "YOBA/UI/traits/selectableElement.h"

namespace YOBA {
	class SelectorItem;

	class Selector : public Layout {
		public:
			Selector();

			Callback<> selectionChanged;
			
			size_t getItemsCount();

			SelectorItem* getItemAt(size_t index);

			int32_t getIndexOfItem(SelectorItem* item);

			void addItem(SelectorItem* item);

			int32_t getSelectedIndex() const;
			SelectorItem* getSelectedItem();

			void setSelectedIndex(int32_t index);

			void setItemsLayout(Layout* layout);

		protected:
			virtual void onSelectionChanged();

		private:
			Layout* _itemsLayout;
			int32_t _selectedIndex = -1;
	};

	class SelectorItem : public Layout, public SelectableElement {
		public:
			Selector* getSelector() const;
			void setSelector(Selector *value);

		protected:
			void onEvent(Event* event) override;

		private:
			Selector* _selector = nullptr;
	};
}