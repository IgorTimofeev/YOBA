#pragma once

#include "element.h"
#include "traits/checkedElement.h"
#include "traits/cornerRadiusElement.h"
#include "traits/focusableElement.h"
#include "main/event.h"
#include "animation.h"

namespace yoba::ui {
	class Switch : public CheckedElement, public CornerRadiusElement, public FocusableElement {
		public:
			Switch() {

			}

			Switch(bool checked) {
				setChecked(checked);
			}

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
				const uint16_t handleOffset = isChecked() ? bounds.getWidth() - bounds.getHeight() : 0;
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

			void onEvent(Event& event) override {
				Element::onEvent(event);

				if (event.getTypeID() != TouchDownEvent::typeID)
					return;

				setChecked(!isChecked());

				event.setHandled(true);
			}

		private:
			const Color* _trackColor;
			const Color* _checkedColor;
			const Color* _handleColor;
	};
}