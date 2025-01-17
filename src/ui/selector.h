#pragma once

#include "layout.h"
#include "vector"
#include "traits/focusableElement.h"

namespace yoba::ui {
	class SelectorItem;

	class Selector : public Layout {
		public:
			Callback<> selectionChanged {};
			
			size_t getItemsCount();

			SelectorItem* getItemAt(size_t index);

			int32_t getIndexOfItem(SelectorItem* item);

			void addItem(SelectorItem* item);

			int32_t getSelectedIndex() const;

			void setSelectedIndex(int32_t index);

			void setItemsLayout(Layout* layout);

		protected:
			virtual void onSelectionChanged();

		private:
			Layout* _itemsLayout = nullptr;
			int32_t _selectedIndex = -1;
	};

	class SelectorItem : public Layout, public FocusableElement {
		public:
			bool isSelected() const;
			virtual void setSelected(bool value);

			Selector *getSelector() const;
			void setSelector(Selector *value);

		protected:
			void onEvent(Event& event) override;

		private:
			Selector* _selector = nullptr;
			bool _isSelected = false;
	};
}