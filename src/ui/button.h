#pragma once

#include "element.h"
#include "../color.h"
#include "../hardware/screen/buffers/screenBuffer.h"
#include "traits/backgroundAware.h"
#include "traits/foregroundAware.h"
#include "traits/textAware.h"
#include "traits/fontAware.h"
#include "traits/cornerRadiusAware.h"
#include "../event.h"

namespace yoba {
	class Button : public TextAware, public FontAware, public BackgroundAware, public ForegroundAware, public CornerRadiusAware {
		public:
			void onRender(ScreenBuffer* screenBuffer) override {
				const auto& bounds = getBounds();

				// Background
				if (getBackground()) {
					screenBuffer->renderFilledRectangle(
						bounds,
						getCornerRadius(),
						isPressed() && getPressedBackground()
							? getPressedBackground()
							: getBackground()
					);
				}

				// Text
				if (getText()) {
					const auto font = getFontOrDefault();

					if (font) {
						auto textSize = font->getSize(getText());

						screenBuffer->renderText(
							Point(
								bounds.getXCenter() - textSize.getWidth() / 2,
								bounds.getYCenter() - textSize.getHeight() / 2
							),
							font,
							isPressed() && getPressedForeground()
								? getPressedForeground()
								: getForeground(),
							getText()
						);
					}
				}
			}

			void onEvent(Event &event) override {
				switch (event.getType()) {
					case EventType::TouchDown:
						setPressed(!isToggle() || !isPressed());

						_onClick.call((TouchEvent&) event);

						break;

					case EventType::TouchUp:
						if (!isToggle())
							setPressed(false);

						break;

					default:
						break;
				}
			}

			void addOnClick(const std::function<void(TouchEvent&)>& value) {
				_onClick.add(value);
			}

			bool isPressed() const {
				return _pressed;
			}

			void setPressed(bool pressed) {
				_pressed = pressed;

				invalidate();
			}

			bool isToggle() const {
				return _toggle;
			}

			void setToggle(bool toggle) {
				_toggle = toggle;
			}

			const Color *getPressedBackground() const {
				return _pressedBackground;
			}

			void setPressedBackground(const Color *pressedBackground) {
				_pressedBackground = pressedBackground;
			}

			const Color *getPressedForeground() const {
				return _pressedForeground;
			}

			void setPressedForeground(const Color *pressedForeground) {
				_pressedForeground = pressedForeground;
			}

		private:
			Action<TouchEvent&> _onClick {};
			bool _pressed = false;
			bool _toggle = false;

			const Color* _pressedBackground = nullptr;
			const Color* _pressedForeground = nullptr;
	};
}
