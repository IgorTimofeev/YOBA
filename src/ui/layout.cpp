#include "layout.h"
#include "main/event.h"
#include <algorithm>

namespace yoba::ui {
	void Layout::onTick() {
		Element::onTick();

		for (auto element : _children)
			element->onTick();
	}

	void Layout::onEvent(Event* event) {
		Element::onEvent(event);

		if (getChildrenCount() == 0 || isCaptured())
			return;

		size_t i = getChildrenCount() - 1;

		while (true) {
			_children[i]->handleEvent(event);

			if (event->isHandled() || i == 0)
				return;

			i--;
		}
	}

	size_t Layout::getChildrenCount() {
		return _children.size();
	}

	int32_t Layout::getIndexOfChild(Element* element) {
		auto iterator = std::find(_children.begin(), _children.end(), element);

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

	void Layout::removeChild(Element* child) {
		auto iterator = std::find(_children.begin(), _children.end(), child);

		if (iterator == _children.end())
			return;

		_children.erase(iterator);

		onChildRemoved(child);
		child->onRemovedFromParent(this);

		invalidate();
	}

	void Layout::removeChildren() {
		for (auto child : _children) {
			onChildRemoved(child);
			child->onRemovedFromParent(this);
		}

		_children.clear();

		invalidate();
	}

	Element* Layout::getChildAt(size_t index) {
		return _children[index];
	}

	void Layout::addChild(Element* child) {
		_children.push_back(child);

		onChildAdded(child);
		child->onAddedToParent(this);

		invalidate();
	}

	Element* Layout::operator[](size_t index) {
		return getChildAt(index);
	}

	void Layout::operator+=(Element* child) {
		addChild(child);
	}

	void Layout::operator-=(Element* child) {
		removeChild(child);
	}

	Size Layout::onMeasure(const Size& availableSize) {
		auto result = Size();

		for (auto child : _children) {
			if (!child->isVisible())
				continue;

			child->measure(availableSize);

			if (child->getMeasuredSize().getWidth() > result.getWidth())
				result.setWidth(child->getMeasuredSize().getWidth());

			if (child->getMeasuredSize().getHeight() > result.getHeight())
				result.setHeight(child->getMeasuredSize().getHeight());
		}

		return result;
	}

	void Layout::onRender(Renderer* renderer, const Bounds& bounds) {
		Element::onRender(renderer, bounds);

		for (auto child : _children) {
			if (child->isVisible()) {
				child->render(renderer, bounds);
			}
		}
	}

	std::vector<Element*>::iterator Layout::begin() {
		return _children.begin();
	}

	std::vector<Element*>::iterator Layout::end() {
		return _children.end();
	}

	void Layout::setApplication(Application* value) {
		Element::setApplication(value);

		for (auto child : _children)
			child->setApplication(value);
	}

	void Layout::onChildAdded(Element* element) {

	}

	void Layout::onChildRemoved(Element* element) {

	}
}