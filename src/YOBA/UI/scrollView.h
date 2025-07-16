#pragma once

#include "scrollBar.h"

#include <YOBA/UI/layout.h>

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

				horizontalScrollBar.setOrientation(Orientation::horizontal);
				horizontalScrollBar.setAlignment(Alignment::stretch, Alignment::end);

				verticalScrollBar.setOrientation(Orientation::vertical);
				verticalScrollBar.setAlignment(Alignment::end, Alignment::stretch);

				setScrollBarSize(3);
				setScrollBarOffset(3);
			}

			ScrollMode getHorizontalScrollMode() const {
				return horizontalScrollMode;
			}

			void setHorizontalScrollMode(const ScrollMode value) {
				horizontalScrollMode = value;

				invalidate();
			}

			ScrollMode getVerticalScrollMode() const {
				return verticalScrollMode;
			}

			void setVerticalScrollMode(const ScrollMode value) {
				verticalScrollMode = value;

				invalidate();
			}

			uint16_t getHorizontalPosition() const {
				return horizontalScrollBar.getPosition();
			}

			void setHorizontalPosition(uint16_t value) {
				const auto& bounds = getBounds();

				if (value > 0) {
					if (contentBounds.getWidth() > bounds.getWidth()) {
						const auto contentBoundsEnd = bounds.getX() - value + contentBounds.getWidth() - 1;

						if (contentBoundsEnd < bounds.getX2()) {
							value = contentBounds.getWidth() - bounds.getWidth();
						}
					}
					else {
						value = 0;
					}
				}

				horizontalScrollBar.setPosition(value);

				invalidate();
			}

			uint16_t getVerticalPosition() const {
				return verticalScrollBar.getPosition();
			}

			void setVerticalPosition(uint16_t value) {
				const auto& bounds = getBounds();

				if (value > 0) {
					if (contentBounds.getHeight() > bounds.getHeight()) {
						const auto contentBoundsEnd = bounds.getY() - value + contentBounds.getHeight() - 1;

						if (contentBoundsEnd < bounds.getY2()) {
							value = contentBounds.getHeight() - bounds.getHeight();
						}
					}
					else {
						value = 0;
					}
				}

				verticalScrollBar.setPosition(value);

				invalidate();
			}

			void setScrollBarOffset(const uint16_t value) {
				horizontalScrollBar.setMargin(Margin(value, 0, value, value));
				verticalScrollBar.setMargin(Margin(0, value, value, value));
			}

			void setScrollBarSize(const uint16_t value) {
				horizontalScrollBar.setSize(Size(Size::computed, value));
				verticalScrollBar.setSize(Size(value, Size::computed));
			}

			void setScrollBarCornerRadius(const uint16_t value) {
				horizontalScrollBar.setCornerRadius(value);
				verticalScrollBar.setCornerRadius(value);
			}

			void setScrollBarThumbColor(const Color* value) {
				horizontalScrollBar.setThumbColor(value);
				verticalScrollBar.setThumbColor(value);
			}

			const Bounds& getContentBounds() const {
				return contentBounds;
			}

			void scrollTo(const Element* element) {
				const auto& elementBounds = element->getBounds();
				const auto& bounds = getBounds();

				// Scroll vertically
				if (elementBounds.getY() < bounds.getY()) {
					const auto delta = elementBounds.getY() - bounds.getY();

					scrollVerticallyBy(delta);
				}
				else if (elementBounds.getY2() > bounds.getY2()) {
					const auto delta = elementBounds.getY2() - bounds.getY2();

					scrollVerticallyBy(delta);
				}
			}

		protected:
			Size onMeasure(const Size& availableSize) override {
				const auto& contentSize = Size(
					horizontalScrollMode == ScrollMode::disabled
					? availableSize.getWidth()
					: Size::unlimited,

					verticalScrollMode == ScrollMode::disabled
					? availableSize.getHeight()
					: Size::unlimited
				);

				contentMeasuredSize.setWidth(0);
				contentMeasuredSize.setHeight(0);

				for (const auto element : *this) {
					if (!element->isVisible())
						continue;

					element->measure(contentSize);

					const auto& elementSize = element->getMeasuredSize();

					if (elementSize.getWidth() > contentMeasuredSize.getWidth())
						contentMeasuredSize.setWidth(elementSize.getWidth());

					if (elementSize.getHeight() > contentMeasuredSize.getHeight())
						contentMeasuredSize.setHeight(elementSize.getHeight());
				}

				horizontalScrollBar.measure(availableSize);
				verticalScrollBar.measure(availableSize);

				return contentMeasuredSize;
			}

			void onRender(Renderer* renderer, const Bounds& bounds) override {
				if (horizontalScrollMode == ScrollMode::disabled) {
					contentBounds.setX(bounds.getX());
					contentBounds.setWidth(bounds.getWidth());
				}
				else {
					contentBounds.setX(bounds.getX() - horizontalScrollBar.getPosition());
					contentBounds.setWidth(contentMeasuredSize.getWidth());
				}

				if (verticalScrollMode == ScrollMode::disabled) {
					contentBounds.setY(bounds.getY());
					contentBounds.setHeight(bounds.getHeight());
				}
				else {
					contentBounds.setY(bounds.getY() - getVerticalPosition());
					contentBounds.setHeight(contentMeasuredSize.getHeight());
				}

				for (const auto element : *this) {
					if (!element->isVisible())
						continue;

					element->render(renderer, contentBounds);
				}

				const auto& processScrollBar = [&bounds, &renderer](ScrollBar& bar, bool& possible, const ScrollMode mode, const uint16_t contentSize, const uint16_t viewportSize) {
					bar.setContentSize(contentSize);
					bar.setViewportSize(viewportSize);
					possible = mode != ScrollMode::disabled && contentSize > viewportSize;

					bar.setVisible(
						mode == ScrollMode::enabled
						|| (mode == ScrollMode::computed && possible)
					);

					if (bar.isVisible())
						bar.render(renderer, bounds);
				};

				processScrollBar(
					horizontalScrollBar,
					horizontalScrollPossible,
					horizontalScrollMode,
					contentBounds.getWidth(),
					bounds.getWidth()
				);

				processScrollBar(
					verticalScrollBar,
					verticalScrollPossible,
					verticalScrollMode,
					contentBounds.getHeight(),
					bounds.getHeight()
				);

				// ESP_LOGI("SCROLL", "contentSize: %f, viewportSize: %f", (float) contentBounds.getHeight(), (float) bounds.getHeight());
			}

			void onCaptureChanged() override {
				Layout::onCaptureChanged();

				if (!isCaptured())
					lastTouchPosition.setX(-1);
			}

			void onEventBeforeChildren(Event* event) override {
				if (event->getTypeID() == PointerDownEvent::typeID) {
					lastTouchPosition = reinterpret_cast<PointerDownEvent*>(event)->getPosition();
				}
				else if (event->getTypeID() == PointerDragEvent::typeID) {
					if (lastTouchPosition.getX() >= 0) {
						const auto position = reinterpret_cast<PointerDragEvent*>(event)->getPosition();
						const auto pointerDelta = position - lastTouchPosition;

						if (isCaptured()) {
							lastTouchPosition = position;

							scrollHorizontallyBy(-pointerDelta.getX());
							scrollVerticallyBy(-pointerDelta.getY());
						}
						else {
							if (pointerDelta.getLength() >= 3) {
								setCaptured(true);

								lastTouchPosition = position;
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

			void scrollHorizontallyBy(const int32_t delta) {
				if (!horizontalScrollPossible)
					return;

				auto position = getHorizontalPosition();

				computeScrollPositionForDelta(
					position,
					delta,
					getBounds().getX2(),
					contentBounds.getX2()
				);

				setHorizontalPosition(position);
			}

			void scrollVerticallyBy(const int32_t delta) {
				if (!verticalScrollPossible)
					return;

				auto position = getVerticalPosition();

				computeScrollPositionForDelta(
					position,
					delta,
					getBounds().getY2(),
					contentBounds.getY2()
				);

				setVerticalPosition(position);
			}

		private:
			ScrollBar horizontalScrollBar {};
			ScrollBar verticalScrollBar {};

			ScrollMode horizontalScrollMode = ScrollMode::disabled;
			ScrollMode verticalScrollMode = ScrollMode::computed;

			Size contentMeasuredSize {};
			Bounds contentBounds {};

			bool horizontalScrollPossible = false;
			bool verticalScrollPossible = false;

			Point lastTouchPosition { -1, -1 };

			static void computeScrollPositionForDelta(uint16_t& position, const int32_t delta, const int32_t boundsPosition2, const int32_t contentPosition2) {
				if (delta > 0) {
					const auto position2Delta = contentPosition2 - boundsPosition2;

					if (position2Delta > 0) {
						position += delta > position2Delta ? position2Delta : delta;
					}
				}
				else {
					if (-delta > position) {
						position = 0;
					}
					else {
						position += delta;
					}
				}
			}
	};
}