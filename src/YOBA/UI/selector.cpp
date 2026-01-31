#include "selector.h"

#include <YOBA/main/events/pointerEvent.h>

namespace YOBA {
	// -------------------------------- Selector --------------------------------

	Selector::Selector() {
		_itemsLayout = this;
	}

	SelectorItem* Selector::getItemAt(const size_t index) const {
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

	void Selector::removeItems() {
		if (!_itemsLayout)
			return;

		_itemsLayout->removeChildren();
		setSelectedIndex(-1);
	}

	void Selector::removeAndDeleteItems() {
		if (!_itemsLayout)
			return;

		_itemsLayout->removeAndDeleteChildren();
		setSelectedIndex(-1);
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

	void Selector::setSelectedIndex(const int32_t index) {
		if (index == _selectedIndex || !_itemsLayout)
			return;

		_selectedIndex = index;

		for (size_t i = 0; i < getItemsCount(); i++) {
			getItemAt(i)->setActive(i == _selectedIndex);
		}

		onSelectionChanged();

		if (_onSelectionChanged)
			_onSelectionChanged();

		invalidate();
	}

	void Selector::onSelectionChanged() {

	}

	size_t Selector::getItemsCount() const {
		return _itemsLayout ? _itemsLayout->getChildrenCount() : 0;
	}

	void Selector::setItemsLayout(Layout *layout) {
		_itemsLayout = layout;
	}

	void Selector::setOnSelectionChanged(const std::function<void()>& onSelectionChanged) {
		_onSelectionChanged = onSelectionChanged;
	}

	// -------------------------------- SelectorItem --------------------------------

	void SelectorItem::onEventBeforeChildren(Event* event) {
		if (event->getTypeID() != PointerUpEvent::typeID)
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
