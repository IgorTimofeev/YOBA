#pragma once

#include "element.h"
#include "cornerRadiusElement.h"
#include "primaryColorElement.h"
#include "secondaryColorElement.h"
#include "textElement.h"

#include "../callback.h"
#include "../event.h"
#include "../hardware/screen/buffers/screenBuffer.h"

namespace yoba {
	class TextField : public PrimaryColorElement, public SecondaryColorElement, public CornerRadiusElement, public TextElement {
		public:
			void onRender(ScreenBuffer* screenBuffer) override;

			void onEvent(InputEvent &event) override;

			void showKeyboard();

			const Color* getKeyboardBackgroundColor() const;
			void setKeyboardBackgroundColor(const Color* keyboardBackgroundColor);

			const Color* getKeyboardTextButtonPrimaryColor() const;
			void setKeyboardTextButtonPrimaryColor(const Color* keyboardTextButtonPrimaryColor);

			const Color* getKeyboardTextButtonSecondaryColor() const;
			void setKeyboardTextButtonSecondaryColor(const Color* keyboardTextButtonSecondaryColor);

			const Color* getKeyboardActionButtonPrimaryColor() const;
			void setKeyboardActionButtonPrimaryColor(const Color* keyboardActionButtonPrimaryColor);

			const Color* getKeyboardActionButtonSecondaryColor() const;
			void setKeyboardActionButtonSecondaryColor(const Color* keyboardActionButtonSecondaryColor);

		private:
			const Color* _keyboardBackgroundColor = nullptr;
			const Color* _keyboardTextButtonPrimaryColor = nullptr;
			const Color* _keyboardTextButtonSecondaryColor = nullptr;
			const Color* _keyboardActionButtonPrimaryColor = nullptr;
			const Color* _keyboardActionButtonSecondaryColor = nullptr;
	};
}
