#pragma once

#include "element.h"
#include "../color.h"
#include "../hardware/screen/buffers/generic/screenBuffer.h"
#include "traits/backgroundColorAware.h"
#include "traits/foregroundColorAware.h"
#include "traits/textAware.h"
#include "traits/fontAware.h"
#include "traits/cornerRadiusAware.h"
#include "../event.h"

namespace yoba {
	class ClickEvent;

	class Button : public TextAware, public FontAware, public BackgroundColorAware, public ForegroundColorAware, public CornerRadiusAware {
		public:
			void onRender(ScreenBuffer* screenBuffer) override;

			void onEvent(InputEvent &event) override;

			bool isPressed() const;
			void setPressed(bool pressed);

			bool isToggle() const;
			void setToggle(bool toggle);

			const Color *getPressedBackground() const;
			void setPressedBackground(const Color *pressedBackground);

			const Color *getPressedForeground() const;
			void setPressedForeground(const Color *pressedForeground);

		private:
			bool _pressed = false;
			bool _toggle = false;

			const Color* _pressedBackground = nullptr;
			const Color* _pressedForeground = nullptr;
	};
}
