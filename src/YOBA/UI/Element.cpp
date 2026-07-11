#include <YOBA/UI/Element.hpp>

#include <assert.h>
#include <algorithm>

#include <YOBA/UI/Animations/Animation.hpp>
#include <YOBA/UI/Layout.hpp>
#include <YOBA/UI/Application.hpp>
#include <YOBA/UI/Transforms/Transform.hpp>
#include <YOBA/Core/Events/ScrollIntoViewEvent.hpp>

namespace YOBA {
	Element::~Element() {
		setCaptured(false);
		setFocused(false);
	}

	bool Element::isVisible() const {
		return _isVisible;
	}

	void Element::setVisible(const bool value) {
		if (value == _isVisible)
			return;

		_isVisible = value;

		invalidate();
	}

	bool Element::isEnabled() const {
		return _isEnabled;
	}

	void Element::setEnabled(const bool value) {
		if (value == _isEnabled)
			return;

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

	void Element::onArrange(const Rectangle& bounds) {

	}

	void Element::onBoundsChanged() {

	}

	void Element::onTick() {

	}

	void Element::measure(const Size& availableSize) {
		const auto& size = getSize();

		if (_layoutTransform) {
			// Processing available size
			_measuredSize = _layoutTransform->processAvailableSizeForMeasure(this, availableSize);

			// Measuring
			_measuredSize = onMeasure(_measuredSize);

			// Setting explicit dimensions
			if (size.getWidth() != Size::computed)
				_measuredSize.setWidth(size.getWidth());

			if (size.getHeight() != Size::computed)
				_measuredSize.setHeight(size.getHeight());

			// Processing measured size
			_measuredSize = _layoutTransform->processMeasuredSize(this, _measuredSize);
		}
		else {
			// Measuring
			_measuredSize = onMeasure(availableSize);

			// Setting explicit dimensions
			if (size.getWidth() != Size::computed)
				_measuredSize.setWidth(size.getWidth());

			if (size.getHeight() != Size::computed)
				_measuredSize.setHeight(size.getHeight());
		}

		// Clamping to min / max dimensions
		_measuredSize.setWidth(std::clamp<int32_t>(
			_measuredSize.getWidth(),
			_minSize.getWidth(),
			_maxSize.getWidth()
		));

		_measuredSize.setHeight(std::clamp<int32_t>(
			 _measuredSize.getHeight(),
			_minSize.getHeight(),
			_maxSize.getHeight()
		));
	}

	void Element::computeDefaultArrangeShit(
		const Alignment alignment,
		const int32_t boundsStart,
		const uint16_t boundsSize,
		
		const uint16_t size,
		const uint16_t measuredSize,

		int32_t& newPosition,
		int32_t& newSize
	) {
		switch (alignment) {
			case Alignment::start:
				newSize = measuredSize;
				newPosition = boundsStart;
				break;

			case Alignment::center:
				newSize = measuredSize;
				newPosition = boundsStart + boundsSize / 2 - newSize / 2;
				break;

			case Alignment::end:
				newSize = measuredSize;
				newPosition = boundsStart + boundsSize - newSize;
				break;

			case Alignment::stretch:
				newSize = size == Size::computed ? boundsSize : measuredSize;
				newPosition = boundsStart;
				break;
		}

		if (newSize < 0)
			newSize = 0;
	}

	void Element::arrange(const Rectangle& parentBounds) {
		const auto& size = getSize();
		const auto& measuredSize = getMeasuredSize();

		// Horizontal
		int32_t newPosition = parentBounds.getX();
		int32_t newSize = parentBounds.getWidth();

		computeDefaultArrangeShit(
			getHorizontalAlignment(),
			parentBounds.getX(),
			parentBounds.getWidth(),

			size.getWidth(),
			measuredSize.getWidth(),

			newPosition,
			newSize
		);

		_layoutBounds.setX(newPosition);
		_layoutBounds.setWidth(static_cast<uint16_t>(newSize));

		// Vertical
		newPosition = parentBounds.getY();
		newSize = parentBounds.getHeight();

		computeDefaultArrangeShit(
			getVerticalAlignment(),
			parentBounds.getY(),
			parentBounds.getHeight(),

			size.getHeight(),
			measuredSize.getHeight(),

			newPosition,
			newSize
		);

		_layoutBounds.setY(newPosition);
		_layoutBounds.setHeight(static_cast<uint16_t>(newSize));

		// Applying layout transform if it presents
		if (_layoutTransform)
			_layoutBounds = _layoutTransform->processLayoutBounds(this, _layoutBounds);

		// Applying rendering transform if it presents
		_renderingBounds =
			_renderingTransform
			? _renderingTransform->computeRenderingBounds(this, _layoutBounds)
			: _layoutBounds;

		onBoundsChanged();

		onArrange(_layoutBounds);
	}

	void Element::render(Renderer* renderer, const Rectangle& bounds) {
		if (_clipToBounds) {
			// Copying clip region to restore it after render pass
			const auto oldClip = renderer->pushClip(_layoutBounds);

			onRender(renderer, _renderingBounds);

			// Restoring clip region
			renderer->setClip(oldClip);
		}
		else {
			onRender(renderer, _renderingBounds);
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

	void Element::scrollIntoView() {
		ScrollIntoViewEvent event { this };
		Application::getCurrent()->pushEvent(&event);
	}

	bool Element::isVisibleOnTarget() const {
		const auto application = Application::getCurrent();

		if (!application)
			return false;

		const auto renderer = application->getRenderer();

		if (!renderer)
			return false;

		return renderer->getClip().intersects(getLayoutBounds());
	}

	Parent* Element::getParent() const {
		return _parent;
	}

	const Size& Element::getMeasuredSize() const {
		return _measuredSize;
	}

	const Rectangle& Element::getLayoutBounds() const {
		return _layoutBounds;
	}

	const Rectangle& Element::getRenderingBounds() const {
		return _renderingBounds;
	}

	Transform* Element::getLayoutTransform() const {
		return _layoutTransform;
	}

	void Element::setLayoutTransform(Transform* transform) {
		if (transform == _layoutTransform)
			return;

		_layoutTransform = transform;

		invalidate();
	}

	Transform* Element::getRenderingTransform() const {
		return _renderingTransform;
	}

	void Element::setRenderingTransform(Transform* transform) {
		if (transform == _renderingTransform)
			return;

		_renderingTransform = transform;

		invalidate();
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

	void Element::invalidateLayout() {
		const auto application = Application::getCurrent();

		if (application)
			application->invalidateLayout();
	}

	void Element::invalidate() {
		const auto application = Application::getCurrent();

		if (application)
			application->invalidate();
	}

	void Element::onRender(Renderer* renderer, const Rectangle& bounds) {

	}

	bool Element::getClipToBounds() const {
		return _clipToBounds;
	}

	void Element::setClipToBounds(const bool value) {
		_clipToBounds = value;
	}

	void Element::addToParent(Parent* parent) {
		assert(_parent == nullptr && "Can't add element to parent, because it already have one");
		assert(_parent != this && "Can't add element to itself");

		_parent = parent;

		onAddedToParent(parent);
	}

	void Element::removeFromParent(Parent* parent) {
		assert(parent == _parent && "Can't remove element from parent that's not related to it");

		_parent = nullptr;

		onRemovedFromParent(parent);
	}

	void Element::onAddedToParent(Parent* parent) {

	}

	void Element::onRemovedFromParent(Parent* parent) {

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