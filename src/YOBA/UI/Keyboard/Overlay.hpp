#pragma once

#include <optional>
#include <functional>

#include <YOBA/UI/Keyboard/Keyboard.hpp>

#include <YOBA/UI/Layout.hpp>
#include <YOBA/UI/RelativeStackLayout.hpp>
#include "YOBA/Core/Events/KeyEvent.hpp"

namespace YOBA {
	class KeyboardOverlay : public RelativeStackLayout {
		public:
			KeyboardOverlay();

			Keyboard* getKeyboard();

			Layout* getTargetLayout() const;
			void setTargetLayout(Layout* value);
			Layout* getTargetLayoutOrApplication() const;

			bool isShown() const;
			void setOnIsShownChanged(const std::function<void()>& callback);

			void show();
			void hide();

		private:
			bool _isShown = false;
			Layout* _targetLayout = nullptr;

			Layout _contentLayout {};
			Keyboard _keyboard {};

			std::function<void()> _onStateChanged = nullptr;
	};
}
