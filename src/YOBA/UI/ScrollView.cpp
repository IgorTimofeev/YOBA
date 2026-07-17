#include <YOBA/UI/ScrollView.hpp>
#include <YOBA/Core/Events/PointerEvent.hpp>
#include <YOBA/Core/Events/MouseWheelEvent.hpp>
#include <YOBA/Core/Events/ScrollIntoViewEvent.hpp>

#include <YOBA/UI/Application.hpp>

namespace YOBA {
	ScrollView::ScrollView() {
		setClipToBounds(true);

		// Horizontal
		_horizontalScrollBar.setOrientation(Orientation::horizontal);
		_horizontalScrollBar.setAlignment(Alignment::stretch, Alignment::end);
		_horizontalScrollBar.setLayoutTransform(&_horizontalTransform);

		// Vertical
		_verticalScrollBar.setOrientation(Orientation::vertical);
		_verticalScrollBar.setAlignment(Alignment::end, Alignment::stretch);
		_verticalScrollBar.setLayoutTransform(&_verticalTransform);

		setScrollBarSize(3);
		setScrollBarOffset(3);
	}

	ScrollMode ScrollView::getHorizontalScrollMode() const {
		return _horizontalScrollMode;
	}

	void ScrollView::setHorizontalScrollMode(const ScrollMode value) {
		_horizontalScrollMode = value;

		invalidate();
	}

	ScrollMode ScrollView::getVerticalScrollMode() const {
		return _verticalScrollMode;
	}

	void ScrollView::setVerticalScrollMode(const ScrollMode value) {
		_verticalScrollMode = value;

		invalidate();
	}

	uint16_t ScrollView::getHorizontalPosition() const {
		return _horizontalScrollBar.getPosition();
	}

	void ScrollView::setHorizontalPosition(const uint16_t value) {
		_horizontalScrollBar.setPosition(value);

		invalidate();
	}

	uint16_t ScrollView::getVerticalPosition() const {
		return _verticalScrollBar.getPosition();
	}

	void ScrollView::setVerticalPosition(const uint16_t value) {
		_verticalScrollBar.setPosition(value);

		invalidate();
	}

	void ScrollView::setScrollBarOffset(const uint16_t value) {
		_verticalTransform.setMargin({ 0, value, value, value });
	}

	void ScrollView::setScrollBarSize(const uint16_t value) {
		_horizontalScrollBar.setSize(Size(Size::computed, value));
		_verticalScrollBar.setSize(Size(value, Size::computed));
	}

	void ScrollView::setScrollBarCornerRadius(const uint16_t value) {
		_horizontalScrollBar.setCornerRadius(value);
		_verticalScrollBar.setCornerRadius(value);
	}

	void ScrollView::setScrollBarThumbColor(const Color* value) {
		_horizontalScrollBar.setThumbColor(value);
		_verticalScrollBar.setThumbColor(value);
	}

	const Rectangle& ScrollView::getContentBounds() const {
		return _contentBounds;
	}

	void ScrollView::scrollTo(const Element* element) {
		const auto& elementBounds = element->getRenderingBounds();
		const auto& bounds = getRenderingBounds();

		// Horizontal
		if (elementBounds.getX() < bounds.getX()) {
			scrollHorizontallyBy(elementBounds.getX() - bounds.getX());
		}
		else if (elementBounds.getX2() > bounds.getX2()) {
			scrollHorizontallyBy(elementBounds.getX2() - bounds.getX2());
		}

		// Vertical
		if (elementBounds.getY() < bounds.getY()) {
			scrollVerticallyBy(elementBounds.getY() - bounds.getY());
		}
		else if (elementBounds.getY2() > bounds.getY2()) {
			scrollVerticallyBy(elementBounds.getY2() - bounds.getY2());
		}
	}

	void ScrollView::scrollToCenter(const Element* element) {
		const auto& elementBounds = element->getRenderingBounds();
		const auto& bounds = getRenderingBounds();

		const auto elementCenter = elementBounds.getCenter();
		const auto center = bounds.getCenter();

		// Horizontal
		scrollHorizontallyBy(elementCenter.getX() - center.getX());
		scrollVerticallyBy(elementCenter.getY() - center.getY());
	}

	Size ScrollView::onMeasure(const Size& availableSize) {
		const auto& contentSize = Size(
			_horizontalScrollMode == ScrollMode::disabled
				? availableSize.getWidth()
				: Size::computed,

			_verticalScrollMode == ScrollMode::disabled
				? availableSize.getHeight()
				: Size::computed
		);

		_contentMeasuredSize.setWidth(0);
		_contentMeasuredSize.setHeight(0);

		const auto child = getChild();

		if (child && child->isVisible()) {
			child->measure(contentSize);

			const auto& childSize = child->getMeasuredSize();

			if (childSize.getWidth() > _contentMeasuredSize.getWidth())
				_contentMeasuredSize.setWidth(childSize.getWidth());

			if (childSize.getHeight() > _contentMeasuredSize.getHeight())
				_contentMeasuredSize.setHeight(childSize.getHeight());
		}

		_horizontalScrollBar.measure(availableSize);
		_verticalScrollBar.measure(availableSize);

		return _contentMeasuredSize;
	}

	void ScrollView::onArrangePass(const Rectangle& bounds) {
		if (_horizontalScrollMode == ScrollMode::disabled) {
			if (_horizontalScrollBar.getPosition() > 0)
				_horizontalScrollBar.setPosition(0);

			_contentBounds.setX(bounds.getX());
			_contentBounds.setWidth(bounds.getWidth());
		}
		else {
			if (_horizontalScrollBar.getPosition() > 0) {
				if (_contentMeasuredSize.getWidth() > bounds.getWidth()) {
					if (bounds.getX() + _contentMeasuredSize.getWidth() - 1 - _horizontalScrollBar.getPosition() < bounds.getX2())
						_horizontalScrollBar.setPosition(_contentMeasuredSize.getWidth() - bounds.getWidth());
				}
				else {
					_horizontalScrollBar.setPosition(0);
				}
			}

			_contentBounds.setX(bounds.getX() - _horizontalScrollBar.getPosition());
			_contentBounds.setWidth(_contentMeasuredSize.getWidth());
		}

		if (_verticalScrollMode == ScrollMode::disabled) {
			if (_verticalScrollBar.getPosition() > 0)
				_verticalScrollBar.setPosition(0);

			_contentBounds.setY(bounds.getY());
			_contentBounds.setHeight(bounds.getHeight());
		}
		else {
			if (_verticalScrollBar.getPosition() > 0) {
				// If content height > scroll view height
				if (_contentMeasuredSize.getHeight() > bounds.getHeight()) {
					// If content Y2 with position < scroll view Y2
					if (bounds.getY() + _contentMeasuredSize.getHeight() - 1 - _verticalScrollBar.getPosition() < bounds.getY2())
						_verticalScrollBar.setPosition(_contentMeasuredSize.getHeight() - bounds.getHeight());
				}
				else {
					_verticalScrollBar.setPosition(0);
				}
			}

			_contentBounds.setY(bounds.getY() - _verticalScrollBar.getPosition());
			_contentBounds.setHeight(_contentMeasuredSize.getHeight());
		}

		const auto child = getChild();

		if (child && child->isVisible())
			child->arrange(_contentBounds);

		const auto& processScrollBar = [&bounds, this](ScrollBar& bar, const ScrollMode mode, const uint16_t contentSize, const uint16_t viewportSize) {
			bar.setContentSize(contentSize);
			bar.setViewportSize(viewportSize);

			switch (mode) {
				case ScrollMode::enabledOnInteraction:
					bar.setVisible(isCaptured() && contentSize > viewportSize);
					break;

				case ScrollMode::enabledOnOverflow:
					bar.setVisible(contentSize > viewportSize);
					break;

				case ScrollMode::enabledAlways:
					bar.setVisible(true);
					break;

				case ScrollMode::enabledAndHidden:
					bar.setVisible(false);
					break;

				case ScrollMode::disabled:
					bar.setVisible(false);
					break;
			}

			if (bar.isVisible())
				bar.arrange(bounds);
		};

		processScrollBar(
			_horizontalScrollBar,
			_horizontalScrollMode,
			_contentBounds.getWidth(),
			bounds.getWidth()
		);

		processScrollBar(
			_verticalScrollBar,
			_verticalScrollMode,
			_contentBounds.getHeight(),
			bounds.getHeight()
		);
	}

	void ScrollView::onArrange(const Rectangle& bounds) {
		// 1st arrange pass
		onArrangePass(bounds);

		if (!_scrollIntoViewLaterTo)
			return;

		// FR FR 100% need to scroll
		// if (!bounds.contains(_scrollIntoViewLaterTo->getLayoutBounds())) {
			scrollToCenter(_scrollIntoViewLaterTo);

			// 2nd arrange pass
			onArrangePass(bounds);
		// }

		_scrollIntoViewLaterTo = nullptr;
	}

	void ScrollView::onRender(Renderer* renderer, const Rectangle& bounds) {
		Decorator::onRender(renderer, bounds);

		const auto& processScrollBar = [&bounds, renderer](ScrollBar& bar) {
			if (bar.isVisible())
				bar.render(renderer, bounds);
		};

		processScrollBar(_horizontalScrollBar);
		processScrollBar(_verticalScrollBar);
	}

	void ScrollView::onCaptureChanged() {
		Decorator::onCaptureChanged();

		if (!isCaptured())
			_lastTouchPosition.setX(-1);
	}

	void ScrollView::onEventBeforeChild(Event* event) {
		if (const auto pointerDownEvent = event->as<PointerDownEvent>()) {
			_lastTouchPosition = pointerDownEvent->getPosition();
		}
		else if (const auto pointerDragEvent = event->as<PointerDragEvent>()) {
			if (_lastTouchPosition.getX() >= 0) {
				const auto position = pointerDragEvent->getPosition();
				const auto pointerDelta = position - _lastTouchPosition;

				if (isCaptured()) {
					_lastTouchPosition = position;

					scrollHorizontallyBy(-pointerDelta.getX());
					scrollVerticallyBy(-pointerDelta.getY());
				}
				else {
					if (pointerDelta.getLength() >= 3) {
						_lastTouchPosition = position;

						setCaptured(true);
					}
				}
			}
		}
		else if (const auto mouseWheelEvent = event->as<MouseWheelEvent>()) {
			scrollVerticallyBy(-mouseWheelEvent->getDelta());
		}
		else if (const auto scrollIntoViewEvent = event->as<ScrollIntoViewEvent>()) {
			_scrollIntoViewLaterTo = scrollIntoViewEvent->getElement();

			event->setHandled(true);
		}
	}

	void ScrollView::onEventAfterChild(Event* event) {
		if (event->is<PointerUpEvent>()) {
			setCaptured(false);
		}
	}

	void ScrollView::scrollHorizontallyBy(const int32_t delta) {
		setHorizontalPosition(static_cast<uint16_t>(std::max(
			static_cast<int32_t>(getHorizontalPosition()) + delta,
			static_cast<int32_t>(0)
		)));
	}

	void ScrollView::scrollVerticallyBy(const int32_t delta) {
		setVerticalPosition(static_cast<uint16_t>(std::max(
			static_cast<int32_t>(getVerticalPosition()) + delta,
			static_cast<int32_t>(0)
		)));
	}
}
