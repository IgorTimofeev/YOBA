#include "layout.h"

namespace yoba {
	void Layout::tick() {
		for (const auto& child : *this) {
			child->setParent(this);
			child->setWorkspace(getWorkspace());

			child->tick();
		}
	}

	void Layout::onRender(ScreenBuffer* screenBuffer) {
		for (const auto& child : *this) {
			child->render(screenBuffer);
		}
	}

	void Layout::onEvent(Event &event) {
		for (int32_t i = (int32_t) getChildrenCount() - 1; i >= 0; i--) {
			getChildAt(i)->handleEvent(event);

			if (event.isHandled())
				return;
		}
	}

	std::vector<Element *>::iterator Layout::begin() {
		return _children.begin();
	}

	std::vector<Element *>::iterator Layout::end() {
		return _children.end();
	}

	size_t Layout::getChildrenCount() {
		return _children.size();
	}

	int32_t Layout::getIndexOfChild(Element *element) {
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
		child->setWorkspace(getWorkspace());

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

		Size childSize;

		for (auto child : *this) {
			if (!child->isVisible())
				continue;

			childSize = child->measure(screenBuffer, availableSize);

			if (childSize.getWidth() > result.getWidth())
				result.setWidth(childSize.getWidth());

			if (childSize.getHeight() > result.getHeight())
				result.setHeight(childSize.getHeight());
		}

		return result;
	}

	void Layout::onArrange(const Bounds &bounds) {
		for (auto child : *this) {
			if (child->isVisible()) {
				child->arrange(bounds);
			}
		}
	}
}