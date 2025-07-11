#pragma once

#include "scrollBar.h"

#include <YOBA/UI/layout.h>
#include <esp_log.h>

namespace YOBA {
	enum class ScrollMode : uint8_t {
		computed,
		enabled,
		hidden,
		disabled
	};

	class ScrollView : public Layout {
		public:
			ScrollView() {
				setClipToBounds(true);

				_horizontalScrollBar.setOrientation(Orientation::horizontal);
				_horizontalScrollBar.setAlignment(Alignment::stretch, Alignment::end);

				_verticalScrollBar.setOrientation(Orientation::vertical);
				_verticalScrollBar.setAlignment(Alignment::end, Alignment::stretch);

				setScrollBarSize(3);
				setScrollBarOffset(3);
			}

			ScrollMode getHorizontalScrollMode() const {
				return _horizontalScrollMode;
			}

			void setHorizontalScrollMode(ScrollMode value) {
				_horizontalScrollMode = value;

				invalidate();
			}

			ScrollMode getVerticalScrollMode() const {
				return _verticalScrollMode;
			}

			void setVerticalScrollMode(ScrollMode value) {
				_verticalScrollMode = value;

				invalidate();
			}

			uint16_t getHorizontalPosition() const {
				return _horizontalScrollBar.getPosition();
			}

			void setHorizontalPosition(uint16_t value) {
				_horizontalScrollBar.setPosition(value);

				invalidate();
			}

			uint16_t getVerticalPosition() const {
				return _verticalScrollBar.getPosition();
			}

			void setVerticalPosition(uint16_t value) {
				_verticalScrollBar.setPosition(value);

				invalidate();
			}

			void setScrollBarOffset(uint16_t value) {
				_horizontalScrollBar.setMargin(Margin(value, 0, value, value));
				_verticalScrollBar.setMargin(Margin(0, value, value, value));
			}

			void setScrollBarSize(uint16_t value) {
				_horizontalScrollBar.setSize(Size(Size::computed, value));
				_verticalScrollBar.setSize(Size(value, Size::computed));
			}

			void setScrollBarCornerRadius(uint16_t value) {
				_horizontalScrollBar.setCornerRadius(value);
				_verticalScrollBar.setCornerRadius(value);
			}

			void setScrollBarThumbColor(const Color* value) {
				_horizontalScrollBar.setThumbColor(value);
				_verticalScrollBar.setThumbColor(value);
			}

		protected:
			Size onMeasure(const Size& availableSize) override {
				const auto& contentSize = Size(
					_horizontalScrollMode == ScrollMode::disabled
					? availableSize.getWidth()
					: Size::unlimited,

					_verticalScrollMode == ScrollMode::disabled
					? availableSize.getHeight()
					: Size::unlimited
				);

				auto result = Size();

				for (const auto element : *this) {
					if (!element->isVisible())
						continue;

					element->measure(contentSize);

					const auto& elementSize = element->getMeasuredSize();

					if (elementSize.getWidth() > result.getWidth())
						result.setWidth(elementSize.getWidth());

					if (elementSize.getHeight() > result.getHeight())
						result.setHeight(elementSize.getHeight());
				}

				_horizontalScrollBar.measure(availableSize);
				_verticalScrollBar.measure(availableSize);

				return result;
			}

			void onRender(Renderer* renderer, const Bounds& bounds) override {
				const auto& measuredSize = getMeasuredSize();

				if (_horizontalScrollMode == ScrollMode::disabled) {
					_contentBounds.setX(bounds.getX());
					_contentBounds.setWidth(bounds.getWidth());
				}
				else {
					if (_horizontalScrollBar.getPosition() > 0) {
						if (measuredSize.getWidth() > bounds.getWidth()) {
							const int32_t contentBoundsX2 = bounds.getX() - _horizontalScrollBar.getPosition() + measuredSize.getWidth() - 1;

							if (contentBoundsX2 < bounds.getX2()) {
								_horizontalScrollBar.setPosition(measuredSize.getWidth() - bounds.getWidth());
							}
						}
						else {
							_horizontalScrollBar.setPosition(0);
						}
					}

					_contentBounds.setX(bounds.getX() - _horizontalScrollBar.getPosition());
					_contentBounds.setWidth(measuredSize.getWidth());
				}

				if (_verticalScrollMode == ScrollMode::disabled) {
					_contentBounds.setY(bounds.getY());
					_contentBounds.setHeight(bounds.getHeight());
				}
				else {
					if (_verticalScrollBar.getPosition() > 0) {
						if (measuredSize.getHeight() > bounds.getHeight()) {
							const int32_t contentBoundsY2 = bounds.getY() - _verticalScrollBar.getPosition() + measuredSize.getHeight() - 1;

							if (contentBoundsY2 < bounds.getY2()) {
								_verticalScrollBar.setPosition(measuredSize.getHeight() - bounds.getHeight());
							}
						}
						else {
							_verticalScrollBar.setPosition(0);
						}
					}

					_contentBounds.setY(bounds.getY() - _verticalScrollBar.getPosition());
					_contentBounds.setHeight(measuredSize.getHeight());
				}

				for (const auto element : *this) {
					if (!element->isVisible())
						continue;

					element->render(renderer, _contentBounds);
				}

				const auto& processScrollBar = [&bounds, &renderer](ScrollBar& bar, bool& possible, ScrollMode mode, uint16_t size, uint16_t contentSize) {
					bar.setTotalSize(contentSize);
					bar.setViewportSize(size);
					possible = mode != ScrollMode::disabled && contentSize > size;

					bar.setVisible(
						mode == ScrollMode::enabled
						|| (mode == ScrollMode::computed && possible)
					);

					if (bar.isVisible())
						bar.render(renderer, bounds);
				};

				processScrollBar(
					_horizontalScrollBar,
					_horizontalScrollPossible,
					_horizontalScrollMode,
					bounds.getWidth(),
					_contentBounds.getWidth()
				);

				processScrollBar(
					_verticalScrollBar,
					_verticalScrollPossible,
					_verticalScrollMode,
					bounds.getHeight(),
					_contentBounds.getHeight()
				);
			}

			void onCaptureChanged() override {
				Layout::onCaptureChanged();

				if (!isCaptured())
					_lastTouchPosition.setX(-1);
			}

			void onEventBeforeChildren(Event* event) override {
				if (event->getTypeID() == PointerDownEvent::typeID) {
					_lastTouchPosition = reinterpret_cast<PointerDownEvent*>(event)->getPosition();
				}
				else if (event->getTypeID() == PointerDragEvent::typeID) {
					if (_lastTouchPosition.getX() >= 0) {
						const auto position = reinterpret_cast<PointerDragEvent*>(event)->getPosition();
						const auto pointerDelta = position - _lastTouchPosition;

						if (isCaptured()) {
							_lastTouchPosition = position;

							if (_horizontalScrollPossible)
								scrollHorizontallyBy(-pointerDelta.getX());

							if (_verticalScrollPossible)
								scrollVerticallyBy(-pointerDelta.getY());
						}
						else {
							if (pointerDelta.getLength() >= 3) {
								setCaptured(true);

								_lastTouchPosition = position;
							}
						}
					}
				}
			}

			void onEventAfterChildren(Event* event) override {
				if (event->getTypeID() == PointerUpEvent::typeID) {
					setCaptured(false);
				}
			}

			void scrollHorizontallyBy(int32_t delta) {
				if (_horizontalScrollMode == ScrollMode::disabled)
					return;

				auto position = getHorizontalPosition();

				computeScrollPositionForDelta(
					position,
					delta,
					getBounds().getX2(),
					_contentBounds.getX2()
				);

				setHorizontalPosition(position);
			}

			void scrollVerticallyBy(int32_t delta) {
				if (_verticalScrollMode == ScrollMode::disabled)
					return;

				auto position = getVerticalPosition();

				computeScrollPositionForDelta(
					position,
					delta,
					getBounds().getY2(),
					_contentBounds.getY2()
				);

				setVerticalPosition(position);
			}

		private:
			ScrollBar _horizontalScrollBar = ScrollBar();
			ScrollBar _verticalScrollBar = ScrollBar();

			ScrollMode _horizontalScrollMode = ScrollMode::disabled;
			ScrollMode _verticalScrollMode = ScrollMode::computed;

			Bounds _contentBounds = Bounds();

			bool _horizontalScrollPossible = false;
			bool _verticalScrollPossible = false;

			Point _lastTouchPosition = Point(-1, -1);

			static void computeScrollPositionForDelta(uint16_t& position, int32_t delta, int32_t boundsPosition2, int32_t contentPosition2) {
				if (delta <= 0) {
					if (-delta > position) {
						position = 0;
					}
					else {
						position += delta;
					}
				}
				else {
					const auto position2Delta = contentPosition2 - boundsPosition2;

					if (position2Delta > 0) {
						position += (delta > position2Delta ? position2Delta : delta);
					}
				}
			}
	};
}