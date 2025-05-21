#include "YOBA/UI/element.h"

#include <algorithm>
#include <esp_log.h>

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

	uint16_t Element::computeMeasureShit(
		uint16_t size,
		uint16_t desiredSize,
		int32_t marginStart,
		int32_t marginEnd,
		uint16_t min,
		uint16_t max
	) {
		int32_t newSize = 0;

		if (size == Size::computed) {
			newSize = desiredSize;
		}
		else {
			newSize = size;
		}

		if (newSize < min) {
			newSize = min;
		}
		else if (newSize > max) {
			newSize = max;
		}
				
		return newSize + marginStart + marginEnd;
	}

	void Element::onBoundsChanged() {

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

		// Horizontal
		_measuredSize.setWidth(computeMeasureShit(
			size.getWidth(),
			_measuredSize.getWidth(),
			margin.getLeft(),
			margin.getRight(),
			_minSize.getWidth(),
			_maxSize.getWidth()
		));

		// Vertical
		_measuredSize.setHeight(computeMeasureShit(
			size.getHeight(),
			_measuredSize.getHeight(),
			margin.getTop(),
			margin.getBottom(),
			_minSize.getHeight(),
			_maxSize.getHeight()
		));
	}

	void Element::computeBoundsShit(
		Alignment alignment,
		int32_t position,
		uint16_t size,

		uint16_t desiredSize,
		int32_t marginStart,
		int32_t marginEnd,

		uint16_t bounds,
		int32_t& newPosition,
		int32_t& newSize
	) {
		switch (alignment) {
			case Alignment::start:
				newSize = desiredSize - marginStart - marginEnd;

				if (newSize < 0)
					newSize = 0;

				newPosition = position + marginStart;

				break;

			case Alignment::center:
				newSize = desiredSize - marginStart - marginEnd;

				if (newSize < 0)
					newSize = 0;

				newPosition = position + marginStart - marginEnd + bounds / 2 - newSize / 2;

				break;

			case Alignment::end:
				newSize = desiredSize - marginStart - marginEnd;

				if (newSize < 0)
					newSize = 0;

				newPosition = position + bounds - marginEnd - newSize;

				break;

			case Alignment::stretch:
				if (size == Size::computed) {
					newSize = bounds;
				}
				else {
					newSize = desiredSize;
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

		// Horizontal
		computeBoundsShit(
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

		// Vertical
		computeBoundsShit(
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

			onRender(renderer);

			// Restoring viewport
			renderer->popViewport(viewport);
		}
		else {
			onRender(renderer);
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
		const auto application = Application::getCurrent();

		return application && application->getFocusedElement() == this;
	}

	void Element::setFocused(bool state) {
		const auto application = Application::getCurrent();

		if (!application || !_focusable || (application->getFocusedElement() == this) == state)
			return;

		application->setFocusedElement(state ? this : nullptr);
	}

	bool Element::isCaptured() const {
		const auto application = Application::getCurrent();

		return application && application->getCapturedElement() == this;
	}

	void Element::setCaptured(bool state) {
		const auto application = Application::getCurrent();

		if (!application || (application->getCapturedElement() == this) == state)
			return;

		application->setCapturedElement(state ? this : nullptr);
	}

	void Element::startAnimation(Animation* animation) {
		const auto application = Application::getCurrent();

		if (application)
			application->startAnimation(animation);
	}

	Layout* Element::getParent() const {
		return _parent;
	}

	const Bounds& Element::getBounds() const {
		return _bounds;
	}

	const Size& Element::getMeasuredSize() const {
		return _measuredSize;
	}

	const Size& Element::getSize() const {
		return _size;
	}

	void Element::setSize(const Size& value) {
		if (value == _size)
			return;

		_size = value;

		invalidate();
	}

	void Element::setWidth(uint16_t value) {
		_size.setWidth(value);

		invalidate();
	}

	void Element::setHeight(uint16_t value) {
		_size.setHeight(value);

		invalidate();
	}

	const Size& Element::getMaxSize() const {
		return _maxSize;
	}

	void Element::setMaxSize(const Size& value) {
		if (value == _maxSize)
			return;

		_maxSize = value;

		invalidate();
	}

	void Element::setMaxWidth(uint16_t value) {
		_maxSize.setWidth(value);

		invalidate();
	}

	void Element::setMaxHeight(uint16_t value) {
		_maxSize.setHeight(value);

		invalidate();
	}

	const Size& Element::getMinSize() const {
		return _minSize;
	}

	void Element::setMinSize(const Size& value) {
		if (value == _minSize)
			return;

		_minSize = value;

		invalidate();
	}

	void Element::setMinWidth(uint16_t value) {
		_minSize.setWidth(value);

		invalidate();
	}

	void Element::setMinHeight(uint16_t value) {
		_minSize.setHeight(value);

		invalidate();
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
		const auto application = Application::getCurrent();

		if (application)
			application->invalidateRender();
	}

	void Element::invalidateMeasure() {
		const auto application = Application::getCurrent();

		if (application)
			application->invalidateMeasure();
	}

	void Element::invalidate() {
		const auto application = Application::getCurrent();

		if (application)
			application->invalidate();
	}

	void Element::onRender(Renderer* renderer) {

	}

	bool Element::getClipToBounds() const {
		return _clipToBounds;
	}

	void Element::setClipToBounds(bool value) {
		_clipToBounds = value;
	}

	void Element::onAddedToParent(Layout* parent) {
		_parent = parent;
	}

	void Element::onRemovedFromParent(Layout* parent) {
		_parent = nullptr;
	}

	void Element::onFocusChanged() {

	}

	void Element::onCaptureChanged() {

	}
}