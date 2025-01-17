#pragma once

#include "element.h"
#include "traits/checkableElement.h"
#include "traits/cornerRadiusElement.h"
#include "traits/focusableElement.h"
#include "event.h"
#include "animation.h"

namespace yoba::ui {
	class Switch : public CheckableElement, public CornerRadiusElement, public FocusableElement {
		public:
			Switch() {

			}

			Switch(bool checked) {
				setChecked(checked);
			}

			const Color* getDefaultTrackColor() const {
				return _defaultTrackColor;
			}

			void setDefaultTrackColor(const Color* value) {
				_defaultTrackColor = value;

				invalidateRender();
			}

			const Color* getCheckedTrackColor() const {
				return _checkedTrackColor;
			}

			void setCheckedTrackColor(const Color* value) {
				_checkedTrackColor = value;

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

				// Checked track
				if (_handleOffset > 0 && _checkedTrackColor) {
					renderer->renderFilledRectangle(
						Bounds(
							bounds.getX(),
							bounds.getY(),
							handleOffsetCenter + handleHalf,
							bounds.getHeight()
						),
						getCornerRadius(),
						_checkedTrackColor
					);
				}

				// Default track
				if (_handleOffset + bounds.getHeight() < bounds.getX2() && _defaultTrackColor) {
					renderer->renderFilledRectangle(
						Bounds(
							bounds.getX() + handleOffsetCenter - handleHalf,
							bounds.getY(),
							bounds.getWidth() - handleOffsetCenter + handleHalf,
							bounds.getHeight()
						),
						getCornerRadius(),
						_defaultTrackColor
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
				CheckableElement::onIsCheckedChanged();

				startAnimation(new Animation(250, [this](double position) {
					const auto& bounds = getBounds();

					_handleOffset = (uint16_t) std::round((isChecked() ? position : 1 - position) * (float) (bounds.getWidth() - bounds.getHeight()));

					invalidateRender();
				}));
			}

		private:
			const Color* _defaultTrackColor;
			const Color* _checkedTrackColor;
			const Color* _handleColor;

			uint16_t _handleOffset = 0;
	};
}