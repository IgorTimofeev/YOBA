#pragma once

#include "element.h"
#include "traits/checkedElement.h"
#include "traits/cornerRadiusElement.h"
#include "traits/focusableElement.h"
#include "event.h"
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
			void onRender(Renderer* renderer) override {
				Element::onRender(renderer);

				const auto& bounds = getBounds();
				const auto handleHalf = bounds.getHeight() / 2;
				const auto handleOffsetCenter = _handleOffset + handleHalf;

				// Checked
				if (_handleOffset > 0 && _checkedColor) {
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
				if (_handleOffset + bounds.getHeight() < bounds.getX2() && _trackColor) {
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
							bounds.getX() + _handleOffset,
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

			void onIsCheckedChanged() override {
				CheckedElement::onIsCheckedChanged();

				startAnimation(new Animation(getApplication(), 250, [this](double position) {
					const auto& bounds = getBounds();

					_handleOffset = (uint16_t) std::round((isChecked() ? position : 1 - position) * (float) (bounds.getWidth() - bounds.getHeight()));

					invalidateRender();
				}));
			}

		private:
			const Color* _trackColor;
			const Color* _checkedColor;
			const Color* _handleColor;

			uint16_t _handleOffset = 0;
	};
}