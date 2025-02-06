#pragma once

#include "element.h"
#include "main/color.h"
#include "rendering/renderers/renderer.h"
#include "traits/textElement.h"
#include "traits/fontElement.h"
#include "traits/cornerRadiusElement.h"
#include "traits/focusableElement.h"
#include "traits/fontScaleElement.h"
#include "main/event.h"

namespace yoba::ui {
	class Button : public FocusableElement, public TextElement, public FontElement, public FontScaleElement, public CornerRadiusElement {
		public:
			Callback<> pressedChanged;

			void onRender(Renderer* renderer, const Bounds& bounds) override;

			void onEvent(Event& event) override;

			bool isPressed() const;
			void setPressed(bool value);

			bool isToggle() const;
			void setToggle(bool value);

			const Color* getDefaultBackgroundColor() const;
			void setDefaultBackgroundColor(const Color* value);

			const Color* getDefaultTextColor() const;
			void setDefaultTextColor(const Color* value);

			const Color* getPressedBackgroundColor() const;
			void setPressedBackgroundColor(const Color* value);

			const Color* getPressedTextColor() const;
			void setPressedTextColor(const Color* value);

		protected:
			virtual void onPressedChanged();

		private:
			bool _pressed = false;
			bool _toggle = false;

			const Color* _defaultBackgroundColor = nullptr;
			const Color* _defaultTextColor = nullptr;

			const Color* _pressedBackgroundColor = nullptr;
			const Color* _pressedTextColor = nullptr;
	};
}
