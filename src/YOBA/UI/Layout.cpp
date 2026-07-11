#include <algorithm>

#include <YOBA/UI/Layout.hpp>
#include <YOBA/UI/Application.hpp>
#include <YOBA/Core/Events/PointerEvent.hpp>
#include <YOBA/Core/Events/PinchEvent.hpp>
#include <cstdint>

namespace YOBA {
	void Layout::onTick() {
		Element::onTick();

		for (const auto element : _children)
			element->onTick();
	}

	size_t Layout::getChildrenCount() const {
		return _children.size();
	}

	bool Layout::hasChildren() const {
		return !_children.empty();
	}

	size_t Layout::getIndexOfChild(Element* element) {
		const auto iterator = std::ranges::find(_children, element);

		if (iterator == _children.end())
			return -1;

		return iterator - _children.begin();
	}

	void Layout::removeChildAt(const int index) {
		const auto child = _children[index];

		_children.erase(_children.begin() + index);

		onChildRemoved(child);
		child->removeFromParent(this);

		invalidate();
	}

	void Layout::removeChild(Element* child) {
		const auto iterator = std::ranges::find(_children, child);

		if (iterator == _children.end())
			return;

		_children.erase(iterator);

		onChildRemoved(child);
		child->removeFromParent(this);

		invalidate();
	}

	void Layout::removeChildren() {
		if (_children.empty())
			return;

		for (const auto child : _children) {
			onChildRemoved(child);
			child->removeFromParent(this);
		}

		_children.clear();

		invalidate();
	}

	void Layout::moveChildrenTo(Layout* layout) {
		const auto childrenCount = _children.size();
		const auto childrenCopy = new Element*[childrenCount];

		for (size_t i = 0; i < childrenCount; i++)
			childrenCopy[i] = _children[i];

		removeChildren();

		for (size_t i = 0; i < childrenCount; i++)
			*layout += childrenCopy[i];

		delete[] childrenCopy;
	}

	void Layout::removeAndDeleteChildren() {
		if (_children.empty())
			return;

		for (const auto child : _children) {
			onChildRemoved(child);
			child->removeFromParent(this);

			delete child;
		}

		_children.clear();

		invalidate();
	}

	Element* Layout::getChildAt(const size_t index) const {
		return _children[index];
	}

	void Layout::addChild(Element* child) {
		_children.push_back(child);

		onChildAdded(child);

		child->addToParent(this);

		invalidate();
	}

	void Layout::insertChild(const size_t index, Element* child) {
		_children.insert(_children.begin() + index, child);

		onChildAdded(child);
		child->addToParent(this);

		invalidate();
	}

	void Layout::insertChildFromEnd(const size_t offset, Element* child) {
		const auto index = static_cast<int32_t>(_children.size()) - static_cast<int32_t>(offset);
		insertChild(index >= 0 ? index : 0, child);
	}

	Element* Layout::operator[](const size_t index) const {
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

	void Layout::onEventBeforeChildren(Event* event) {

	}

	void Layout::onEventAfterChildren(Event* event) {

	}

	void Layout::onParentHandleEventBeforeChildren(Event* event) {
		onEventBeforeChildren(event);
	}

	void Layout::onParentHandlePointerEventForChildren(Event* event, const Rectangle& bounds, bool callHandlers) {
		if (getChildrenCount() == 0)
			return;

		size_t i = getChildrenCount() - 1;

		while (true) {
			_children[i]->handleEvent(event, bounds, callHandlers);

			if (event->isHandled())
				callHandlers = false;

			if (i == 0)
				break;

			i--;
		}
	}

	void Layout::onParentHandleEventForChildren(Event* event, const Rectangle& bounds, const bool callHandlers) {
		if (getChildrenCount() == 0)
			return;

		size_t i = getChildrenCount() - 1;

		while (true) {
			_children[i]->handleEvent(event, bounds, callHandlers);

			if (event->isHandled() || i == 0)
				return;

			i--;
		}
	}

	void Layout::onParentHandleEventAfterChildren(Event* event) {
		onEventAfterChildren(event);
	}

	Size Layout::onMeasure(const Size& availableSize) {
		uint16_t width = 0;
		uint16_t height = 0;

		Size measuredSize;

		for (const auto child : _children) {
			if (!child->isVisible())
				continue;

			child->measure(availableSize);
			measuredSize = child->getMeasuredSize();

			if (measuredSize.getWidth() > width)
				width = measuredSize.getWidth();

			if (measuredSize.getHeight() > height)
				height = measuredSize.getHeight();
		}

		return {
			width,
			height
		};
	}

	void Layout::onArrange(const Rectangle& bounds) {
		for (const auto child : _children) {
			if (child->isVisible()) {
				child->arrange(bounds);
			}
		}
	}

	void Layout::onRender(Renderer* renderer, const Rectangle& bounds) {
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
}
