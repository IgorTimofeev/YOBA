#include "selectorItem.h"
#include "selector.h"

namespace yoba {
	void SelectorItem::onEvent(InputEvent &event) {
		if (!(typeid(event) == typeid(TouchDownEvent) || typeid(event) == typeid(TouchDragEvent)))
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