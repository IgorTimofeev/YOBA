#include "YOBA/UI/element.h"
#include "layout.h"
#include "application.h"
#include "animation.h"
#include "YOBA/main/event.h"

namespace YOBA {
	Element::~Element() {
		setFocused(false);
		setCaptured(false);
	}

	bool Element::isVisible() const {
		return _isVisible;
	}

	void Element::setVisible(bool value) {
		_isVisible = value;

		invalidate();
	}

	bool Element::isEnabled() const {
		return _isEnabled;
	}

	void Element::setEnabled(bool value) {
		_isEnabled = value;

		invalidate();
	}

	Size Element::onMeasure(const Size& availableSize) {
		return { 0, 0 };
	}

	void Element::computeMeasureShit(
		const uint16_t &size,
		const uint16_t &desiredSize,
		const int32_t &marginStart,
		const int32_t &marginEnd,
		int32_t &newSize
	) {
		if (size == Size::computed) {
			newSize = desiredSize;
		}
		else {
			newSize = size;
		}

		newSize = newSize + marginStart + marginEnd;

		if (newSize < 0)
			newSize = 0;
	}

	void Element::onTick() {

	}

	void Element::measure(const Size& availableSize) {
		const auto& size = getSize();
		const auto& margin = getMargin();

		_measuredSize = onMeasure(Size(
			availableSize.getWidth() - margin.getLeft() - margin.getRight(),
			availableSize.getHeight() - margin.getTop() - margin.getBottom()
		));

		int32_t newSize = 0;

		// Width
		computeMeasureShit(
			size.getWidth(),
			_measuredSize.getWidth(),
			margin.getLeft(),
			margin.getRight(),
			newSize
		);

		_measuredSize.setWidth(newSize);

		// Height
		computeMeasureShit(
			size.getHeight(),
			_measuredSize.getHeight(),
			margin.getTop(),
			margin.getBottom(),
			newSize
		);

		_measuredSize.setHeight(newSize);
	}

	void Element::computeArrangeShit(
		const Alignment& alignment,
		const int32_t &position,
		const uint16_t &size,

		const uint16_t &measuredSize,
		const int32_t &marginStart,
		const int32_t &marginEnd,

		const uint16_t &limit,
		int32_t &newPosition,
		int32_t &newSize
	) {
		switch (alignment) {
			case Alignment::start:
				newSize = measuredSize - marginStart - marginEnd;

				if (newSize < 0)
					newSize = 0;

				newPosition = position + marginStart;

				break;

			case Alignment::center:
				newSize = measuredSize - marginStart - marginEnd;

				if (newSize < 0)
					newSize = 0;

				newPosition = position + marginStart - marginEnd + limit / 2 - newSize / 2;

				break;

			case Alignment::end:
				newSize = measuredSize - marginStart - marginEnd;

				if (newSize < 0)
					newSize = 0;

				newPosition = position + limit - marginEnd - newSize;

				break;

			case Alignment::stretch:
				if (size == Size::computed) {
					newSize = limit;
				}
				else {
					newSize = measuredSize;
				}

				newSize = newSize - marginStart - marginEnd;

				if (newSize < 0)
					newSize = 0;

				newPosition = position + marginStart;

				break;
		}
	}

	void Element::render(Renderer* renderer, const Bounds& bounds) {
		const auto& margin = getMargin();
		const auto& measuredSize = getMeasuredSize();
		const auto& size = getSize();

		Bounds newBounds;
		int32_t newPosition = 0;
		int32_t newSize = 0;

		computeArrangeShit(
			getHorizontalAlignment(),
			bounds.getX(),
			size.getWidth(),
			measuredSize.getWidth(),
			margin.getLeft(),
			margin.getRight(),
			bounds.getWidth(),
			newPosition,
			newSize
		);

		newBounds.setX(newPosition);
		newBounds.setWidth(newSize);

		computeArrangeShit(
			getVerticalAlignment(),
			bounds.getY(),
			size.getHeight(),
			measuredSize.getHeight(),
			margin.getTop(),
			margin.getBottom(),
			bounds.getHeight(),
			newPosition,
			newSize
		);

		newBounds.setY(newPosition);
		newBounds.setHeight(newSize);

		_bounds = newBounds;

		onBoundsChanged();

		if (_clipToBounds) {
			// Copying viewport to restore it after render pass
			const auto viewport = renderer->pushViewport(_bounds);

			onRender(renderer, _bounds);

			// Restoring viewport
			renderer->popViewport(viewport);
		}
		else {
			onRender(renderer, _bounds);
		}
	}

	void Element::handleEvent(Event* event) {
		if (!event->matches(this))
			return;

		onEvent(event);
	}

	void Element::onEvent(Event* event) {

	}

	bool Element::isFocusable() const {
		return _focusable;
	}

	void Element::setFocusable(bool focusable) {
		_focusable = focusable;
	}

	bool Element::isFocused() const {
		return _application && _application->getFocusedElement() == this;
	}

	void Element::setFocused(bool value) {
		if (!_application || !_focusable || (_application->getFocusedElement() == this) == value)
			return;

		const auto previousFocusedElement = _application->getFocusedElement();

		_application->setFocusedElement(value ? this : nullptr);

		if (previousFocusedElement && previousFocusedElement != this)
			previousFocusedElement->onFocusChanged();

		onFocusChanged();

		_application->invalidate();
	}

	bool Element::isCaptured() const {
		return _application && _application->getCapturedElement() == this;
	}

	void Element::setCaptured(bool value) {
		if (!_application || (_application->getCapturedElement() == this) == value)
			return;

		const auto previousCapturedElement = _application->getCapturedElement();

		_application->setCapturedElement(value ? this : nullptr);

		if (previousCapturedElement && previousCapturedElement != this)
			previousCapturedElement->onCaptureChanged();

		onCaptureChanged();

		_application->invalidate();
	}

	void Element::startAnimation(Animation* animation) {
		if (_application)
			_application->startAnimation(animation);
	}

	Layout* Element::getParent() const {
		return _parent;
	}

	void Element::setApplication(Application* value) {
		_application = value;
	}

	Application *Element::getApplication() const {
		return _application;
	}

	const Bounds& Element::getBounds() const {
		return _bounds;
	}

	const Size& Element::getMeasuredSize() const {
		return _measuredSize;
	}

	void Element::setSize(const Size& value) {
		if (value == _size)
			return;

		_size = value;

		invalidate();
	}

	const Size& Element::getSize() const {
		return _size;
	}

	void Element::setMargin(const Margin& value) {
		if (value == _margin)
			return;

		_margin = value;

		invalidate();
	}

	const Margin& Element::getMargin() const {
		return _margin;
	}

	void Element::setAlignment(Alignment horizontal, Alignment vertical) {
		if (horizontal == _horizontalAlignment && vertical == _verticalAlignment)
			return;

		_horizontalAlignment = horizontal;
		_verticalAlignment = vertical;

		invalidate();
	}

	void Element::setAlignment(Alignment uniformValue) {
		setAlignment(uniformValue, uniformValue);
	}

	void Element::setVerticalAlignment(Alignment value) {
		if (value == _verticalAlignment)
			return;

		_verticalAlignment = value;

		invalidate();
	}

	Alignment Element::getVerticalAlignment() const {
		return _verticalAlignment;
	}

	void Element::setHorizontalAlignment(Alignment value) {
		if (value == _horizontalAlignment)
			return;

		_horizontalAlignment = value;

		invalidate();
	}

	Alignment Element::getHorizontalAlignment() const {
		return _horizontalAlignment;
	}

	void Element::invalidateRender() {
		if (_application)
			_application->invalidateRender();
	}

	void Element::invalidateLayout() {
		if (_application)
			_application->invalidateLayout();
	}

	void Element::invalidate() {
		if (_application) {
			_application->invalidate();
		}
	}

	void Element::onRender(Renderer* renderer, const Bounds& bounds) {

	}

	bool Element::getClipToBounds() const {
		return _clipToBounds;
	}

	void Element::setClipToBounds(bool value) {
		_clipToBounds = value;
	}

	void Element::setWidth(uint16_t value) {
		_size.setWidth(value);

		invalidate();
	}

	void Element::setHeight(uint16_t value) {
		_size.setHeight(value);

		invalidate();
	}

	void Element::onAddedToParent(Layout* parent) {
		_parent = parent;
		setApplication(_parent->getApplication());
	}

	void Element::onRemovedFromParent(Layout* parent) {
		_parent = nullptr;
	}

	void Element::onFocusChanged() {

	}

	void Element::onCaptureChanged() {

	}

	void Element::onBoundsChanged() {

	}
}