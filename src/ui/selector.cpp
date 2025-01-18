#include "selector.h"

namespace yoba::ui {
	// -------------------------------- Selector --------------------------------

	SelectorItem* Selector::getItemAt(size_t index) {
		return
			_itemsLayout
			? dynamic_cast<SelectorItem*>((*_itemsLayout)[index])
			: nullptr;
	}

	int32_t Selector::getIndexOfItem(SelectorItem* item) {
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

	SelectorItem* Selector::getSelectedItem() {
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

	size_t Selector::getItemsCount() {
		return _itemsLayout ? _itemsLayout->getChildrenCount() : 0;
	}

	void Selector::setItemsLayout(Layout *layout) {
		_itemsLayout = layout;
	}

	// -------------------------------- SelectorItem --------------------------------

	void SelectorItem::onEvent(Event& event) {
		const auto isTouchDown = event.getTypeID() == TouchDownEvent::typeID;
		const auto isTouchDrag = event.getTypeID() == TouchDragEvent::typeID;

		if (!(isTouchDown || isTouchDrag))
			return;

		setFocused(true);

		if (getSelector())
			getSelector()->setSelectedIndex(getSelector()->getIndexOfItem(this));

		event.setHandled(true);
	}

	Selector *SelectorItem::getSelector() const {
		return _selector;
	}

	void SelectorItem::setSelector(Selector *value) {
		_selector = value;
	}

	bool SelectorItem::isSelected() const {
		return _isSelected;
	}

	void SelectorItem::setSelected(bool value) {
		_isSelected = value;
	}
}