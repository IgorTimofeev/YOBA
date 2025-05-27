#include "layout.h"
#include <YOBA/main/event.h>
#include <algorithm>
#include <esp_log.h>

#include "application.h"

namespace YOBA {
	void Layout::onTick() {
		Element::onTick();

		for (const auto element : _children)
			element->onTick();
	}

	void Layout::pushEvent(Event* event) {
		if (!isVisible())
			return;

		const auto callOnEvent = !updateTouchOver(event) || isTouchOver() || isCaptured();

		if (callOnEvent) {
			onEventBeforeChildren(event);

			callScreenEventFunctions(
				this,

				&Layout::onTouchDownBeforeChildren,
				&Layout::onTouchDragBeforeChildren,
				&Layout::onTouchUpBeforeChildren,

				&Layout::onPinchDownBeforeChildren,
				&Layout::onPinchDragBeforeChildren,
				&Layout::onPinchUpBeforeChildren,

				event
			);

			if (event->isHandled())
				return;
		}

		if (getChildrenCount() > 0) {
			size_t i = getChildrenCount() - 1;

			while (true) {
				const auto child = _children[i];

				if (child->isVisible()) {
					child->pushEvent(event);

					if (event->isHandled())
						return;
				}

				if (i == 0)
					break;

				i--;
			}
		}

		if (callOnEvent) {
			onEventAfterChildren(event);

			callScreenEventFunctions(
				this,

				&Layout::onTouchDownAfterChildren,
				&Layout::onTouchDragAfterChildren,
				&Layout::onTouchUpAfterChildren,

				&Layout::onPinchDownAfterChildren,
				&Layout::onPinchDragAfterChildren,
				&Layout::onPinchUpAfterChildren,

				event
			);
		}
	}

	void Layout::onEventBeforeChildren(Event* event) {

	}

	void Layout::onEventAfterChildren(Event* event) {

	}

	void Layout::onTouchDownBeforeChildren(TouchDownEvent* event) {

	}

	void Layout::onTouchDownAfterChildren(TouchDownEvent* event) {

	}

	void Layout::onTouchDragBeforeChildren(TouchDragEvent* event) {

	}

	void Layout::onTouchDragAfterChildren(TouchDragEvent* event) {

	}

	void Layout::onTouchUpBeforeChildren(TouchUpEvent* event) {

	}

	void Layout::onTouchUpAfterChildren(TouchUpEvent* event) {

	}

	void Layout::onPinchDownBeforeChildren(PinchDownEvent* event) {

	}

	void Layout::onPinchDownAfterChildren(PinchDownEvent* event) {

	}

	void Layout::onPinchDragBeforeChildren(PinchDragEvent* event) {

	}

	void Layout::onPinchDragAfterChildren(PinchDragEvent* event) {

	}

	void Layout::onPinchUpBeforeChildren(PinchUpEvent* event) {

	}

	void Layout::onPinchUpAfterChildren(PinchUpEvent* event) {

	}

	size_t Layout::getChildrenCount() const {
		return _children.size();
	}

	size_t Layout::getIndexOfChild(Element* element) {
		const auto iterator = std::ranges::find(_children, element);

		if (iterator == _children.end())
			return -1;

		return iterator - _children.begin();
	}

	void Layout::removeChildAt(int index) {
		const auto child = _children[index];

		_children.erase(_children.begin() + index);

		onChildRemoved(child);
		child->onRemovedFromParent(this);

		invalidate();
	}

	void Layout::removeChild(Element* child) {
		const auto iterator = std::ranges::find(_children, child);

		if (iterator == _children.end())
			return;

		_children.erase(iterator);

		onChildRemoved(child);
		child->onRemovedFromParent(this);

		invalidate();
	}

	void Layout::removeChildren() {
		if (_children.size() == 0)
			return;

		for (const auto child : _children) {
			onChildRemoved(child);
			child->onRemovedFromParent(this);
		}

		_children.clear();

		invalidate();
	}

	void Layout::removeAndDeleteChildren() {
		if (_children.size() == 0)
			return;

		for (const auto child : _children) {
			onChildRemoved(child);
			child->onRemovedFromParent(this);

			delete child;
		}

		_children.clear();

		invalidate();
	}

	Element* Layout::getChildAt(size_t index) const {
		return _children[index];
	}

	void Layout::addChild(Element* child) {
		_children.push_back(child);

		onChildAdded(child);
		child->onAddedToParent(this);

		invalidate();
	}

	void Layout::insertChild(size_t index, Element* child) {
		_children.insert(_children.begin() + index, child);

		onChildAdded(child);
		child->onAddedToParent(this);

		invalidate();
	}

	void Layout::insertChildFromEnd(size_t offset, Element* child) {
		const ssize_t index = _children.size() - offset;
		insertChild(index >= 0 ? index : 0, child);
	}

	Element* Layout::operator[](size_t index) const {
		return getChildAt(index);
	}

	Layout& Layout::operator+=(Element* child) {
		addChild(child);

		return *this;
	}

	Layout& Layout::operator-=(Element* child) {
		removeChild(child);

		return *this;
	}

	Size Layout::onMeasure(const Size& availableSize) {
		auto result = Size();

		for (const auto child : _children) {
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

		for (const auto child : _children) {
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

	void Layout::onChildAdded(Element* child) {

	}

	void Layout::onChildRemoved(Element* child) {

	}
}
