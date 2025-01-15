#pragma once

#include "element.h"
#include "color.h"
#include "rendering/renderer.h"
#include "ui/traits/primaryColorElement.h"
#include "ui/traits/secondaryColorElement.h"
#include "ui/traits/textElement.h"
#include "ui/traits/fontElement.h"
#include "ui/traits/cornerRadiusElement.h"
#include "ui/traits/focusableElement.h"
#include "event.h"

namespace yoba::ui {
	class Button : public FocusableElement, public TextElement, public FontElement, public PrimaryColorElement, public SecondaryColorElement, public CornerRadiusElement {
		public:
			Callback<> pressedChanged {};

			void onRender(Renderer* renderer) override;

			void onEvent(Event& event) override;

			bool isPressed() const;
			void setPressed(bool value);

			bool isToggle() const;
			void setToggle(bool value);

			const Color* getPressedPrimaryColor() const;
			void setPressedPrimaryColor(const Color* value);

			const Color* getPressedSecondaryColor() const;
			void setPressedSecondaryColor(const Color* value);

		protected:
			virtual void onPressedChanged();

		private:
			bool _pressed = false;
			bool _toggle = false;

			const Color* _pressedPrimaryColor = nullptr;
			const Color* _pressedSecondaryColor = nullptr;
	};
}
