#pragma once

#include <functional>

#include <YOBA/UI/Control.hpp>
#include <YOBA/Core/Colors.hpp>
#include <YOBA/Rendering/Renderers/Renderer.hpp>
#include <YOBA/UI/Traits/CornerRadiusElement.hpp>
#include <YOBA/UI/Traits/ActiveElement.hpp>

namespace YOBA {
	class Button :
		public Control,
		public ActiveElement,
		public CornerRadiusElement
	{
		public:
			bool isToggle() const;
			void setToggle(bool value);

			const Color* getDefaultBackgroundColor() const;
			void setDefaultBackgroundColor(const Color* value);

			const Color* getActiveBackgroundColor() const;
			void setActiveBackgroundColor(const Color* value);

			const Color* getDefaultBorderColor() const;

			void setDefaultBorderColor(const Color* defaultBorderColor);

			const Color* getActiveBorderColor() const;
			void setActiveBorderColor(const Color* pressedBorderColor);

			void setOnClick(const std::function<void()>& onClick);

		protected:
			void onEvent(Event* event) override;
			void onPointerOverChanged() override;

			void onRender(Renderer* renderer, const Rectangle& bounds) override;

			virtual void onClick();

		private:
			const Color* _defaultBackgroundColor = nullptr;
			const Color* _defaultBorderColor = nullptr;

			const Color* _activeBackgroundColor = nullptr;
			const Color* _activeBorderColor = nullptr;

			bool _isToggle = false;
			bool _pointerWasDown = false;
			bool _previousIsActive = false;

			std::function<void()> _onClick {};

			void callOnClick();
	};
}