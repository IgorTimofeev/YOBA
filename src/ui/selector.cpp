#include "selector.h"

namespace yoba {
	// -------------------------------- Selector --------------------------------

	SelectorItem* Selector::getItemAt(size_t index) {
		return
			_itemsLayout
			? (SelectorItem*) (*_itemsLayout)[index]
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

	void Selector::setSelectedIndex(int32_t index) {
		if (index == _selectedIndex || !_itemsLayout)
			return;

		_selectedIndex = index;

		for (size_t i = 0; i < _itemsLayout->getChildrenCount(); i++) {
			((SelectorItem*) (*_itemsLayout)[i])->setSelected(i == index);
		}

		onSelectionChanged();

		_onSelectionChanged.call();
	}

	void Selector::onSelectionChanged() {

	}

	size_t Selector::getItemsCount() {
		return _itemsLayout ? _itemsLayout->getChildrenCount() : 0;
	}

	void Selector::setItemsLayout(Layout *layout) {
		_itemsLayout = layout;
	}

	const Callback<>& Selector::getOnSelectionChanged() const {
		return _onSelectionChanged;
	}

	// -------------------------------- SelectorItem --------------------------------

	void SelectorItem::onEvent(InputEvent &event) {
		if (!(event.getTypeID() == TouchDownEvent::typeID || event.getTypeID() == TouchDragEvent::typeID))
			return;

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
}