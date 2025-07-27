#include "layout.h"
#include <algorithm>
#include <esp_log.h>

#include <YOBA/UI/application.h>
#include <YOBA/main/events/pointerEvent.h>
#include <YOBA/main/events/pinchEvent.h>


namespace YOBA {
	void Layout::onTick() {
		Element::onTick();

		for (const auto element : _children)
			element->onTick();
	}

	void Layout::pushEvent(Event* event) {
		handleEvent(event, !event->isHandled());
	}

	void Layout::onEventBeforeChildren(Event* event) {

	}

	void Layout::onEventAfterChildren(Event* event) {

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
		const auto index = static_cast<ssize_t>(_children.size()) - static_cast<ssize_t>(offset);
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

	void Layout::handleEvent(Event* event, bool callHandlers) {
		const auto isPointer = PointerEvent::isPointer(event);
		const auto isPinch = PinchEvent::isPinch(event);

		if (isPointer || isPinch) {
			bool callHandlersOfThis;

			if (isVisible() && isVisibleForPointerEvents()) {
				bool intersects;

				if (isPointer) {
					intersects = getBounds().contains(reinterpret_cast<PointerEvent*>(event)->getPosition());
				}
				else {
					const auto& bounds = getBounds();
					const auto pinchEvent = reinterpret_cast<PinchEvent*>(event);
					intersects = bounds.contains(pinchEvent->getPosition1()) && bounds.contains(pinchEvent->getPosition2());
				}

				const auto capturedElement = Application::getCurrent() ? Application::getCurrent()->getCapturedElement() : nullptr;

				setPointerOver(
					event->getTypeID() != PointerUpEvent::typeID
					&& event->getTypeID() != PinchUpEvent::typeID
					&& intersects
					&& (
						!capturedElement
						|| capturedElement == this
					)
				);

				if (isEnabled()) {
					callHandlersOfThis =
						callHandlers
						&& (
							(
								intersects
								&& !capturedElement
							)
							|| capturedElement == this
						);
				}
				else {
					callHandlers = false;
					callHandlersOfThis = false;
				}
			}
			else {
				setPointerOver(false);

				callHandlers = false;
				callHandlersOfThis = false;
			}

			// Before
			if (callHandlersOfThis) {
				onEventBeforeChildren(event);

				if (event->isHandled()) {
					callHandlers = false;
					callHandlersOfThis = false;
				}
			}

			// Children
			if (getChildrenCount() > 0) {
				size_t i = getChildrenCount() - 1;

				while (true) {
					_children[i]->handleEvent(event, callHandlers);

					if (event->isHandled()) {
						callHandlers = false;
						callHandlersOfThis = false;
					}

					if (i == 0)
						break;

					i--;
				}
			}

			// After
			if (callHandlersOfThis)
				onEventAfterChildren(event);
		}
		else {
			if (!isVisible() || !isEnabled())
				return;

			const auto capturedElement = Application::getCurrent() ? Application::getCurrent()->getCapturedElement() : nullptr;
			const auto callHandlersOnThis = callHandlers && (!capturedElement || capturedElement == this);

			// Before
			if (callHandlersOnThis)
				onEventBeforeChildren(event);

			if (event->isHandled())
				return;

			// Children
			if (getChildrenCount() > 0) {
				size_t i = getChildrenCount() - 1;

				while (true) {
					_children[i]->handleEvent(event, callHandlers);

					if (event->isHandled())
						return;

					if (i == 0)
						break;

					i--;
				}
			}

			// After
			if (callHandlersOnThis)
				onEventAfterChildren(event);
		}
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
