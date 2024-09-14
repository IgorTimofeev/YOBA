#include "selector_item.h"
#include "selector.h"

namespace yoba {
	void SelectorItem::onEvent(Event &event) {
		if ((event.getType() != EventType::touchDown && event.getType() != EventType::touchDrag))
			return;

		if (getSelector())
			getSelector()->setSelectedIndex(getSelector()->getIndexOfItem(this));

		event.setHandled(true);
	}

	void SelectorItem::setSelected(const bool &value) {

	}

	Selector *SelectorItem::getSelector() const {
		return _selector;
	}

	void SelectorItem::setSelector(Selector *value) {
		_selector = value;
	}
}