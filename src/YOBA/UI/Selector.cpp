#include <YOBA/UI/Selector.hpp>
#include <YOBA/UI/ElementImplementation.hpp>
#include <YOBA/Core/Events/PointerEvent.hpp>

namespace YOBA {
	// -------------------------------- Selector --------------------------------

	Selector::Selector() {
		_itemLayout = this;
	}

	SelectorItem* Selector::getItemAt(const size_t index) const {
		return
			_itemLayout
			? dynamic_cast<SelectorItem*>((*_itemLayout)[index])
			: nullptr;
	}

	int32_t Selector::getIndexOfItem(SelectorItem* item) const {
		return
			_itemLayout
			? _itemLayout->getIndexOfChild(item)
			: -1;
	}

	void Selector::addItem(SelectorItem* item) const {
		if (!_itemLayout)
			return;

		*_itemLayout += item;
	}

	void Selector::removeItems() {
		if (!_itemLayout)
			return;

		_itemLayout->removeChildren();
		setSelectedIndex(-1);
	}

	void Selector::removeAndDeleteItems() {
		if (!_itemLayout)
			return;

		_itemLayout->removeAndDeleteChildren();
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
		if (index == _selectedIndex || !_itemLayout)
			return;

		_selectedIndex = index;

		SelectorItem* item;

		for (size_t i = 0; i < getItemCount(); i++) {
			item = getItemAt(i);

			if (item)
				item->setActive(i == _selectedIndex);
		}

		onSelectionChanged();

		if (_onSelectionChanged)
			_onSelectionChanged();

		invalidate();
	}

	void Selector::onSelectionChanged() {

	}

	size_t Selector::getItemCount() const {
		return _itemLayout ? _itemLayout->getChildrenCount() : 0;
	}

	void Selector::setItemsLayout(Layout *layout) {
		_itemLayout = layout;
	}

	void Selector::setOnSelectionChanged(const std::function<void()>& onSelectionChanged) {
		_onSelectionChanged = onSelectionChanged;
	}

	// -------------------------------- SelectorItem --------------------------------

	void SelectorItem::onAddedToParent(Parent* parent) {
		_selectorUpdated = false;
	}

	void SelectorItem::onEventBeforeChildren(Event* event) {
		if (event->getTypeID() != PointerUpEvent::typeID)
			return;

		setFocused(true);

		if (getSelector())
			getSelector()->setSelectedIndex(getSelector()->getIndexOfItem(this));

		event->setHandled(true);
	}

	Selector* SelectorItem::getSelector() {
		if (_selectorUpdated)
			return _selector;

		_selector = findParent<Selector>();

		if (_selector)
			_selectorUpdated = true;

		return _selector;
	}
}
