#pragma once

#include <functional>

#include <YOBA/UI/Layout.hpp>
#include <YOBA/UI/Traits/ActiveElement.hpp>

#include "Selector.hpp"

namespace YOBA {
	class SelectorItem;

	class Selector : public Layout {
		public:
			Selector();

			size_t getItemCount() const;
			SelectorItem* getItemAt(size_t index) const;
			int32_t getIndexOfItem(SelectorItem* item) const;

			void addItem(SelectorItem* item) const;
			void removeItems();
			void removeAndDeleteItems();

			int32_t getSelectedIndex() const;
			SelectorItem* getSelectedItem() const;

			void setSelectedIndex(int32_t index);

			void setItemsLayout(Layout* layout);

			void setOnSelectionChanged(const std::function<void()>& onSelectionChanged);

		protected:
			virtual void onSelectionChanged();

		private:
			Layout* _itemLayout;
			int32_t _selectedIndex = -1;
			std::function<void()> _onSelectionChanged = nullptr;
	};

	class SelectorItem : public Layout, public ActiveElement {
		public:
			Selector* getSelector();

		protected:
			void onAddedToParent(Parent* parent) override;
			void onEventBeforeChildren(Event* event) override;

		private:
			Selector* _selector = nullptr;
			bool _selectorUpdated = false;
	};
}
