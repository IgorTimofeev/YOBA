#pragma once

#include "element.h"
#include "../color.h"
#include "hardware/screen/buffers/screenBuffer.h"
#include "primaryColorElement.h"
#include "secondaryColorElement.h"
#include "textElement.h"
#include "fontElement.h"
#include "cornerRadiusElement.h"
#include "../event.h"

namespace yoba {
	class ClickEvent;

	class Button : public TextElement, public FontElement, public PrimaryColorElement, public SecondaryColorElement, public CornerRadiusElement {
		public:
			void onRender(ScreenBuffer* screenBuffer) override;

			void onEvent(InputEvent &event) override;

			bool isPressed() const;
			void setPressed(bool value);

			bool isToggle() const;
			void setToggle(bool value);

			const Color *getPressedPrimaryColor() const;
			void setPressedPrimaryColor(const Color *value);

			const Color *getPressedSecondaryColor() const;
			void setPressedSecondaryColor(const Color *value);

			Callback<>& getOnClick();

			void setOnClick(const Callback<>& onClick);

		private:
			bool _pressed = false;
			bool _toggle = false;

			const Color* _pressedPrimaryColor = nullptr;
			const Color* _pressedSecondaryColor = nullptr;

			Callback<> _onClick {};
	};
}
