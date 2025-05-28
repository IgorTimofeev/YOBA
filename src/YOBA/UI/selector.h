#pragma once

#include <YOBA/UI/layout.h>
#include <YOBA/UI/traits/activeElement.h>

namespace YOBA {
	class SelectorItem;

	class Selector : public Layout {
		public:
			Selector();

			Callback<> selectionChanged;
			
			size_t getItemsCount() const;

			SelectorItem* getItemAt(size_t index) const;

			int32_t getIndexOfItem(SelectorItem* item) const;

			void addItem(SelectorItem* item);
			void removeItems();
			void removeAndDeleteItems();

			int32_t getSelectedIndex() const;
			SelectorItem* getSelectedItem() const;

			void setSelectedIndex(int32_t index);
			void setItemsLayout(Layout* layout);

		protected:
			virtual void onSelectionChanged();

		private:
			Layout* _itemsLayout;
			int32_t _selectedIndex = -1;
	};

	class SelectorItem : public Layout, public ActiveElement {
		public:
			Selector* getSelector() const;
			void setSelector(Selector *value);

		protected:
			void onEventBeforeChildren(Event* event) override;

		private:
			Selector* _selector = nullptr;
	};
}