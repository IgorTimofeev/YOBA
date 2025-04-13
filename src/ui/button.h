#pragma once

#include "element.h"
#include "main/color.h"
#include "rendering/renderers/renderer.h"
#include "traits/textElement.h"
#include "traits/fontElement.h"
#include "traits/cornerRadiusElement.h"
#include "traits/fontScaleElement.h"
#include "traits/checkedElement.h"
#include "traits/borderElement.h"
#include "main/event.h"

namespace yoba::ui {
	class Button : public CheckedElement, public TextElement, public FontElement, public FontScaleElement, public CornerRadiusElement {
		public:
			Callback<> click {};

			void onRender(Renderer* renderer, const Bounds& bounds) override;

			void onEvent(Event* event) override;

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

			const Color* getDefaultBorderColor() const;

			void setDefaultBorderColor(const Color* defaultBorderColor);

			const Color* getPressedBorderColor() const;
			void setPressedBorderColor(const Color* pressedBorderColor);

			const Margin& getContentMargin() const;
			void setContentMargin(const Margin& contentMargin);

		protected:
			virtual void onClick();

		private:
			bool _toggle = false;

			Margin _contentMargin = Margin(0);

			const Color* _defaultBackgroundColor = nullptr;
			const Color* _defaultBorderColor = nullptr;
			const Color* _defaultTextColor = nullptr;

			const Color* _pressedBackgroundColor = nullptr;
			const Color* _pressedBorderColor = nullptr;
			const Color* _pressedTextColor = nullptr;

			void callOnClick();
	};
}
