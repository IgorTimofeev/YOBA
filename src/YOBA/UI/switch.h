#pragma once

#include <YOBA/UI/control.h>
#include <YOBA/UI/traits/activeElement.h>
#include <YOBA/UI/traits/cornerRadiusElement.h>
#include <YOBA/main/event.h>

namespace YOBA {
	class Switch : public Control, public ActiveElement, public CornerRadiusElement {
		public:
			Switch() = default;

			explicit Switch(bool checked) {
				setActive(checked);
			}

			Callback<> isActiveChanged {};

			const Color* getTrackColor() const {
				return _trackColor;
			}

			void setTrackColor(const Color* value) {
				_trackColor = value;

				invalidateRender();
			}

			const Color* getCheckedColor() const {
				return _checkedColor;
			}

			void setCheckedColor(const Color* value) {
				_checkedColor = value;

				invalidateRender();
			}

			const Color* getHandleColor() const {
				return _handleColor;
			}

			void setHandleColor(const Color* value) {
				_handleColor = value;

				invalidateRender();
			}

		protected:
			void onRender(Renderer* renderer, const Bounds& bounds) override {
				const uint16_t handleHalf = bounds.getHeight() / 2;
				const uint16_t handleOffset = isActive() ? bounds.getWidth() - bounds.getHeight() : 0;
				const uint16_t handleOffsetCenter = handleOffset + handleHalf;

				// Checked
				if (handleOffset > 0 && _checkedColor) {
					renderer->renderFilledRectangle(
						Bounds(
							bounds.getX(),
							bounds.getY(),
							handleOffsetCenter + handleHalf,
							bounds.getHeight()
						),
						getCornerRadius(),
						_checkedColor
					);
				}

				// Track
				if (handleOffset + bounds.getHeight() < bounds.getX2() && _trackColor) {
					renderer->renderFilledRectangle(
						Bounds(
							bounds.getX() + handleOffsetCenter - handleHalf,
							bounds.getY(),
							bounds.getWidth() - handleOffsetCenter + handleHalf,
							bounds.getHeight()
						),
						getCornerRadius(),
						_trackColor
					);
				}

				// Handle
				if (_handleColor) {
					renderer->renderFilledRectangle(
						Bounds(
							bounds.getX() + handleOffset,
							bounds.getY(),
							bounds.getHeight(),
							bounds.getHeight()
						),
						getCornerRadius(),
						_handleColor
					);
				}
			}

			void onTouchUp(TouchUpEvent* event) override {
				setActive(!isActive());

				event->setHandled(true);
			}

			void onIsActiveChanged() override {
				ActiveElement::onIsActiveChanged();

				isActiveChanged();
			}

		private:
			const Color* _trackColor = nullptr;
			const Color* _checkedColor = nullptr;
			const Color* _handleColor = nullptr;
	};
}