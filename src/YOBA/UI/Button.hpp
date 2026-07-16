#pragma once

#include <functional>

#include <YOBA/UI/Control.hpp>
#include <YOBA/Core/Colors.hpp>
#include <YOBA/Rendering/Renderers/Renderer.hpp>
#include <YOBA/Core/Margin.hpp>
#include <YOBA/UI/Traits/TextElement.hpp>
#include <YOBA/UI/Traits/FontElement.hpp>
#include <YOBA/UI/Traits/CornerRadiusElement.hpp>
#include <YOBA/UI/Traits/FontScaleElement.hpp>
#include <YOBA/UI/Traits/ActiveElement.hpp>

namespace YOBA {
	class Button :
		public Control,
		public ActiveElement,
		public TextElement,
		public FontElement,
		public FontScaleElement,
		public CornerRadiusElement
	{
		public:
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

			void setOnClick(const std::function<void()>& onClick);

		protected:
			void onEvent(Event* event) override;
			void onPointerOverChanged() override;

			void onRender(Renderer* renderer, const Rectangle& bounds) override;

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
			bool _pointerWasDown = false;
			bool _previousIsActive = false;

			std::function<void()> _onClick {};

			void callOnClick();
	};
}
