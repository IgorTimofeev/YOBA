#include "selector.h"

namespace YOBA {
	// -------------------------------- Selector --------------------------------

	Selector::Selector() {
		_itemsLayout = this;
	}

	SelectorItem* Selector::getItemAt(size_t index) const {
		return
			_itemsLayout
			? dynamic_cast<SelectorItem*>((*_itemsLayout)[index])
			: nullptr;
	}

	int32_t Selector::getIndexOfItem(SelectorItem* item) const {
		return
			_itemsLayout
			? _itemsLayout->getIndexOfChild(item)
			: -1;
	}

	void Selector::addItem(SelectorItem* item) {
		if (!_itemsLayout)
			return;

		item->setSelector(this);
		*_itemsLayout += item;
	}

	int32_t Selector::getSelectedIndex() const {
		return _selectedIndex;
	}

	SelectorItem* Selector::getSelectedItem() const {
		return
			_selectedIndex >= 0
			? getItemAt(_selectedIndex)
			: nullptr;
	}

	void Selector::setSelectedIndex(int32_t index) {
		if (index == _selectedIndex || !_itemsLayout)
			return;

		_selectedIndex = index;

		for (size_t i = 0; i < _itemsLayout->getChildrenCount(); i++) {
			getItemAt(i)->setSelected(i == index);
		}

		onSelectionChanged();

		selectionChanged();
	}

	void Selector::onSelectionChanged() {

	}

	size_t Selector::getItemsCount() const {
		return _itemsLayout ? _itemsLayout->getChildrenCount() : 0;
	}

	void Selector::setItemsLayout(Layout *layout) {
		_itemsLayout = layout;
	}

	// -------------------------------- SelectorItem --------------------------------

	void SelectorItem::onEvent(Event* event) {
		if (!(event->getTypeID() == TouchDownEvent::typeID || event->getTypeID() == TouchDragEvent::typeID))
			return;

		setFocused(true);

		if (getSelector())
			getSelector()->setSelectedIndex(getSelector()->getIndexOfItem(this));

		event->setHandled(true);
	}

	Selector* SelectorItem::getSelector() const {
		return _selector;
	}

	void SelectorItem::setSelector(Selector *value) {
		_selector = value;
	}
}