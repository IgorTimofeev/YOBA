#pragma once

#include <YOBA/UI/element.h>
#include <YOBA/main/color.h>
#include <YOBA/main/rendering/renderer.h>
#include <YOBA/main/event.h>
#include <YOBA/UI/traits/textElement.h>
#include <YOBA/UI/traits/fontElement.h>
#include <YOBA/UI/traits/cornerRadiusElement.h>
#include <YOBA/UI/traits/fontScaleElement.h>
#include <YOBA/UI/traits/activeElement.h>

namespace YOBA {
	class Button :
		public ActiveElement,
		public TextElement,
		public FontElement,
		public FontScaleElement,
		public CornerRadiusElement
	{
		public:
			Callback<> click {};
			Callback<> isActiveChanged {};

			bool isToggle() const;
			void setToggle(bool value);

			const Color* getDefaultBackgroundColor() const;
			void setDefaultBackgroundColor(const Color* value);

			const Color* getDefaultTextColor() const;
			void setDefaultTextColor(const Color* value);

			const Color* getActiveBackgroundColor() const;
			void setActiveBackgroundColor(const Color* value);

			const Color* getActiveTextColor() const;
			void setActiveTextColor(const Color* value);

			const Color* getDefaultBorderColor() const;

			void setDefaultBorderColor(const Color* defaultBorderColor);

			const Color* getActiveBorderColor() const;
			void setActiveBorderColor(const Color* pressedBorderColor);

			const Margin& getContentMargin() const;
			void setContentMargin(const Margin& contentMargin);

		protected:
			void onRender(Renderer* renderer, const Bounds& bounds) override;

			void onEvent(Event* event) override;

			void onIsActiveChanged() override;

			virtual void onClick();

		private:
			Margin _contentMargin = Margin(0);

			const Color* _defaultBackgroundColor = nullptr;
			const Color* _defaultBorderColor = nullptr;
			const Color* _defaultTextColor = nullptr;

			const Color* _activeBackgroundColor = nullptr;
			const Color* _activeBorderColor = nullptr;
			const Color* _activeTextColor = nullptr;

			bool _isToggle = false;
			bool _wasDown = false;
			bool _previousIsActive = false;

			void callOnClick();
	};
}
