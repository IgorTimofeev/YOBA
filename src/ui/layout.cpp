#include "layout.h"
#include "../event.h"

namespace yoba {
	void Layout::onRender(ScreenBuffer* screenBuffer) {
		for (const auto& child : _children) {
			child->render(screenBuffer);
		}
	}

	void Layout::onEvent(InputEvent &event) {
		for (int32_t i = (int32_t) getChildrenCount() - 1; i >= 0; i--) {
			getChildAt(i)->handleEvent(event);

			if (event.isHandled())
				return;
		}
	}

	void Layout::setRoot(Application* value) {
		Element::setRoot(value);

		for (const auto& child : _children) {
			child->setRoot(value);
		}
	}

	size_t Layout::getChildrenCount() {
		return _children.size();
	}

	int32_t Layout::getIndexOfChild(Element* element) {
		auto iterator = find(_children.begin(), _children.end(), element);

		if (iterator == _children.end()) {
			return -1;
		}
		else {
			return iterator - _children.begin();
		}
	}

	void Layout::removeChildAt(int index) {
		_children.erase(_children.begin() + index);

		invalidate();
	}

	void Layout::removeChild(Element *child) {
		auto iterator = std::find(_children.begin(), _children.end(), child);

		if (iterator == _children.end())
			return;

		_children.erase(iterator);

		invalidate();
	}

	void Layout::removeChildren() {
		_children.clear();

		invalidate();
	}

	Element *Layout::getChildAt(size_t index) {
		return _children[index];
	}

	void Layout::addChild(Element *child) {
		child->setParent(this);
		child->setRoot(getRoot());

		_children.push_back(child);

		invalidate();
	}

	Element *Layout::operator[](size_t index) {
		return getChildAt(index);
	}

	void Layout::operator+=(Element *child) {
		addChild(child);
	}

	void Layout::operator-=(Element *child) {
		removeChild(child);
	}

	Size Layout::onMeasure(ScreenBuffer* screenBuffer, const Size &availableSize) {
		auto result = Size();

		for (auto child : _children) {
			if (!child->isVisible())
				continue;

			child->measure(screenBuffer, availableSize);

			if (child->getMeasuredSize().getWidth() > result.getWidth())
				result.setWidth(child->getMeasuredSize().getWidth());

			if (child->getMeasuredSize().getHeight() > result.getHeight())
				result.setHeight(child->getMeasuredSize().getHeight());
		}

		return result;
	}

	void Layout::onArrange(const Bounds &bounds) {
		for (auto child : _children) {
			if (child->isVisible()) {
				child->arrange(bounds);
			}
		}
	}
}