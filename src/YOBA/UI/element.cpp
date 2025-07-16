#include <YOBA/UI/element.h>

#include <assert.h>
#include <algorithm>
#include <esp_log.h>
#include <esp_debug_helpers.h>

#include <YOBA/UI/animation.h>
#include <YOBA/UI/layout.h>
#include <YOBA/UI/application.h>

namespace YOBA {
	Element::~Element() {
		setCaptured(false);
		setFocused(false);
	}

	bool Element::isVisible() const {
		return _isVisible;
	}

	void Element::setVisible(const bool value) {
		_isVisible = value;

		invalidate();
	}

	bool Element::isEnabled() const {
		return _isEnabled;
	}

	void Element::setEnabled(const bool value) {
		_isEnabled = value;

		invalidate();
	}

	bool Element::isVisibleForPointerEvents() const {
		return _isVisibleForPointerEvents;
	}

	void Element::setVisibleForPointerEvents(const bool value) {
		_isVisibleForPointerEvents = value;
	}

	Size Element::onMeasure(const Size& availableSize) {
		return { 0, 0 };
	}

	uint16_t Element::computeMeasureShit(
		const uint16_t size,
		const uint16_t desiredSize,
		const int32_t marginStart,
		const int32_t marginEnd,
		const uint16_t min,
		const uint16_t max
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
			static_cast<uint16_t>(std::max<int32_t>(static_cast<int32_t>(availableSize.getWidth()) - margin.getLeft() - margin.getRight(), 0)),
			static_cast<uint16_t>(std::max<int32_t>(static_cast<int32_t>(availableSize.getHeight()) - margin.getTop() - margin.getBottom(), 0))
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
		const Alignment alignment,
		const int32_t position,
		const uint16_t size,

		const uint16_t desiredSize,
		const int32_t marginStart,
		const int32_t marginEnd,

		const uint16_t bounds,
		int32_t& newPosition,
		int32_t& newSize
	) {
		switch (alignment) {
			case Alignment::start:
				newSize = static_cast<int32_t>(desiredSize) - marginStart - marginEnd;

				if (newSize < 0)
					newSize = 0;

				newPosition = position + marginStart;

				break;

			case Alignment::center:
				newSize = static_cast<int32_t>(desiredSize) - marginStart - marginEnd;

				if (newSize < 0)
					newSize = 0;

				newPosition = position + marginStart - marginEnd + bounds / 2 - newSize / 2;

				break;

			case Alignment::end:
				newSize = static_cast<int32_t>(desiredSize) - marginStart - marginEnd;

				if (newSize < 0)
					newSize = 0;

				newPosition = position + bounds - marginEnd - newSize;

				break;

			case Alignment::stretch:
				if (size == Size::computed) {
					newSize = bounds;
				}
				else {
					newSize = static_cast<int32_t>(desiredSize);
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

			onRender(renderer, _bounds);

			// Restoring viewport
			renderer->popViewport(viewport);
		}
		else {
			onRender(renderer, _bounds);
		}
	}

	bool Element::isFocusable() const {
		return _focusable;
	}

	void Element::setFocusable(const bool focusable) {
		_focusable = focusable;
	}

	bool Element::isFocused() const {
		const auto application = Application::getCurrent();

		return application && application->getFocusedElement() == this;
	}

	void Element::setFocused(const bool state) {
		const auto application = Application::getCurrent();

		if (!application || !_focusable || (application->getFocusedElement() == this) == state)
			return;

		application->setFocusedElement(state ? this : nullptr);
	}

	bool Element::isCaptured() const {
		const auto application = Application::getCurrent();

		return application && application->getCapturedElement() == this;
	}

	void Element::setCaptured(const bool state) {
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

	void Element::setWidth(const uint16_t value) {
		_size.setWidth(value);

		invalidate();
	}

	void Element::setHeight(const uint16_t value) {
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

	void Element::setMaxWidth(const uint16_t value) {
		_maxSize.setWidth(value);

		invalidate();
	}

	void Element::setMaxHeight(const uint16_t value) {
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

	void Element::setMinWidth(const uint16_t value) {
		_minSize.setWidth(value);

		invalidate();
	}

	void Element::setMinHeight(const uint16_t value) {
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

	void Element::setAlignment(const Alignment horizontal, const Alignment vertical) {
		if (horizontal == _horizontalAlignment && vertical == _verticalAlignment)
			return;

		_horizontalAlignment = horizontal;
		_verticalAlignment = vertical;

		invalidate();
	}

	void Element::setAlignment(const Alignment uniformValue) {
		setAlignment(uniformValue, uniformValue);
	}

	void Element::setVerticalAlignment(const Alignment value) {
		if (value == _verticalAlignment)
			return;

		_verticalAlignment = value;

		invalidate();
	}

	Alignment Element::getVerticalAlignment() const {
		return _verticalAlignment;
	}

	void Element::setHorizontalAlignment(const Alignment value) {
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

	void Element::onRender(Renderer* renderer, const Bounds& bounds) {

	}

	bool Element::getClipToBounds() const {
		return _clipToBounds;
	}

	void Element::setClipToBounds(const bool value) {
		_clipToBounds = value;
	}

	void Element::addToParent(Layout* parent) {
		assert(_parent == nullptr && "Element already has a parent. Remove it first.");

		_parent = parent;

		onAddedToParent(parent);
	}

	void Element::removeFromParent(Layout* parent) {
		assert(parent == _parent && "Attempted to remove element from non-attached parent");

		_parent = nullptr;

		onRemovedFromParent(parent);
	}

	void Element::onAddedToParent(Layout* parent) {

	}

	void Element::onRemovedFromParent(Layout* parent) {

	}

	void Element::onFocusChanged() {

	}

	void Element::onCaptureChanged() {

	}

	// -------------------------------- Events --------------------------------

	void Element::setPointerOver(const bool value) {
		if (value == _isPointerOver)
			return;

		_isPointerOver = value;
		onPointerOverChanged();
	}

	bool Element::isPointerOver() const {
		return _isPointerOver;
	}

	void Element::onPointerOverChanged() {

	}
}