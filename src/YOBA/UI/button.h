#pragma once

#include "YOBA/UI/element.h"
#include "YOBA/main/color.h"
#include "YOBA/main/rendering/renderer.h"
#include "YOBA/main/event.h"
#include "YOBA/UI/traits/textElement.h"
#include "YOBA/UI/traits/fontElement.h"
#include "YOBA/UI/traits/cornerRadiusElement.h"
#include "YOBA/UI/traits/fontScaleElement.h"
#include "YOBA/UI/traits/checkedElement.h"
#include "YOBA/UI/traits/borderElement.h"

namespace YOBA {
	enum class ButtonCheckMode : uint8_t {
		normal,
		toggle,
		manual
	};

	class Button : public CheckedElement, public TextElement, public FontElement, public FontScaleElement, public CornerRadiusElement {
		public:
			Callback<> click {};

			void onRender(Renderer* renderer, const Bounds& bounds) override;

			void onEvent(Event* event) override;

			ButtonCheckMode getCheckMode() const;
			void setCheckMode(ButtonCheckMode value);

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
			Margin _contentMargin = Margin(0);

			const Color* _defaultBackgroundColor = nullptr;
			const Color* _defaultBorderColor = nullptr;
			const Color* _defaultTextColor = nullptr;

			const Color* _pressedBackgroundColor = nullptr;
			const Color* _pressedBorderColor = nullptr;
			const Color* _pressedTextColor = nullptr;

			ButtonCheckMode _checkMode = ButtonCheckMode::normal;

			void callOnClick();
	};
}
