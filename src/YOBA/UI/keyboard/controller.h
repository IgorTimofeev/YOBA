#pragma once

#include <optional>
#include <cstdint>

#include <YOBA/UI/keyboard/keyboard.h>

#include <YOBA/main/key.h>
#include <YOBA/main/events/keyEvent.h>

#include <YOBA/UI/traits/fontElement.h>
#include <YOBA/UI/shapes/rectangle.h>
#include <YOBA/UI/layout.h>
#include <YOBA/UI/button.h>
#include <YOBA/UI/relativeStackLayout.h>

namespace YOBA {
	class KeyboardControllerLayout : public RelativeStackLayout {
		public:
			KeyboardControllerLayout();

			Keyboard keyboard {};
			Layout temporaryLayout {};
	};

	class KeyboardController {
		public:
			static void show(const uint8_t layoutOptions);
			static void hide();
			static bool isVisible();

			static void setOnShow(const std::optional<std::function<void(Keyboard*)>>& value);
			static Keyboard* getKeyboard();

			static Layout* getTargetLayout();
			static void setTargetLayout(Layout* value);

		private:
			static std::optional<std::function<void(Keyboard*)>> _onKeyboardShow;
			static KeyboardControllerLayout* _controllerLayout;
			static Layout* _targetLayout;

			static Layout* getTargetLayoutOrApplication();
	};
}