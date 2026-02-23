#pragma once

#include <optional>

#include <YOBA/UI/keyboard/keyboard.h>

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

			static void setOnShow(const std::function<void(Keyboard*)>& value);
			static Keyboard* getKeyboard();

			static Layout* getTargetLayout();
			static void setTargetLayout(Layout* value);

		private:
			static std::function<void(Keyboard*)> _onKeyboardShow;
			static KeyboardControllerLayout* _controllerLayout;
			static Layout* _targetLayout;

			static Layout* getTargetLayoutOrApplication();
	};
}