#include <YOBA/UI/Decorator.hpp>
#include <YOBA/UI/Application.hpp>
#include <YOBA/Core/Events/PointerEvent.hpp>
#include <YOBA/Core/Events/PinchEvent.hpp>

namespace YOBA {
	Element* Decorator::getChild() const {
		return _child;
	}

	void Decorator::setChild(Element* child) {
		if (child == _child)
			return;

		if (_child)
			onChildRemoved(_child);

		_child = child;

		if (_child)
			onChildAdded(child);
	}

	bool Decorator::hasChild() const {
		return !!_child;
	}

	Decorator& Decorator::operator+=(Element* child) {
		setChild(child);

		return *this;
	}

	Decorator& Decorator::operator-=(Element*) {
		setChild(nullptr);

		return *this;
	}

	void Decorator::onTick() {
		if (!_child)
			return;

		_child->onTick();
	}

	Size Decorator::onMeasure(const Size& availableSize) {
		if (!_child || !_child->isVisible())
			return {};

		_child->measure(availableSize);

		return _child->getMeasuredSize();
	}

	void Decorator::onArrange(const Rectangle& bounds) {
		if (!_child || !_child->isVisible())
			return;

		_child->arrange(bounds);
	}

	void Decorator::onRender(Renderer* renderer, const Rectangle& bounds) {
		if (!_child || !_child->isVisible())
			return;

		_child->onRender(renderer, bounds);
	}

	void Decorator::onEventBeforeChild(Event* event) {

	}

	void Decorator::onEventAfterChild(Event* event) {

	}

	void Decorator::onParentHandleEventBeforeChildren(Event* event) {
		onEventBeforeChild(event);
	}

	void Decorator::onParentHandlePointerEventForChildren(Event* event, const Rectangle& bounds, const bool callHandlers) {
		if (!_child)
			return;

		_child->handleEvent(event, bounds, callHandlers);
	}

	void Decorator::onParentHandleEventForChildren(Event* event, const Rectangle& bounds, const bool callHandlers) {
		if (!_child)
			return;

		_child->handleEvent(event, bounds, callHandlers);
	}

	void Decorator::onParentHandleEventAfterChildren(Event* event) {
		onEventAfterChild(event);
	}
}
