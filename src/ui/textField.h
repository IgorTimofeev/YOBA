#pragma once

#include "element.h"
#include "cornerRadiusElement.h"
#include "primaryColorElement.h"
#include "secondaryColorElement.h"
#include "textElement.h"
#include "fontElement.h"

#include "../callback.h"
#include "../event.h"
#include "../hardware/screen/buffers/screenBuffer.h"

namespace yoba {
	class TextField : public PrimaryColorElement, public SecondaryColorElement, public CornerRadiusElement, public TextElement, public FontElement {
		public:
			void onRender(ScreenBuffer* screenBuffer) override;

			void onEvent(InputEvent &event) override;

			void showKeyboard();

			const Color* getKeyboardBackgroundColor() const;
			void setKeyboardBackgroundColor(const Color* keyboardBackgroundColor);

			const Color* getKeyboardDefaultButtonPrimaryColor() const;
			void setKeyboardDefaultButtonPrimaryColor(const Color* keyboardTextButtonPrimaryColor);

			const Color* getKeyboardDefaultButtonSecondaryColor() const;
			void setKeyboardDefaultButtonSecondaryColor(const Color* keyboardTextButtonSecondaryColor);

			const Color* getKeyboardActionButtonPrimaryColor() const;
			void setKeyboardActionButtonPrimaryColor(const Color* keyboardActionButtonPrimaryColor);

			const Color* getKeyboardActionButtonSecondaryColor() const;
			void setKeyboardActionButtonSecondaryColor(const Color* keyboardActionButtonSecondaryColor);

			const Font* getKeyboardFont() const;
			void setKeyboardFont(const Font* keyboardFont);

		private:
			const Font* _keyboardFont = nullptr;
			const Color* _keyboardBackgroundColor = nullptr;
			const Color* _keyboardDefaultButtonPrimaryColor = nullptr;
			const Color* _keyboardDefaultButtonSecondaryColor = nullptr;
			const Color* _keyboardActionButtonPrimaryColor = nullptr;
			const Color* _keyboardActionButtonSecondaryColor = nullptr;
	};
}
