#pragma once

#include <YOBA/UI/Control.hpp>
#include <YOBA/UI/Traits/ActiveElement.hpp>
#include <YOBA/UI/Traits/CornerRadiusElement.hpp>
#include <YOBA/UI/Animations/CallbackAnimation.hpp>

namespace YOBA {
	class Switch : public Control, public ActiveElement, public CornerRadiusElement {
		public:
			Switch(const bool isActive);
			Switch();

			const Color* getTrackColor() const;
			void setTrackColor(const Color* value);

			const Color* getActiveColor() const;
			void setActiveColor(const Color* value);

			const Color* getHandleColor() const;
			void setHandleColor(const Color* value);

			uint32_t getAnimationDuration() const;
			void setAnimationDuration(const uint32_t durationUs);

		protected:
			void onEvent(Event* event) override;
			void onIsActiveChanged() override;
			void onRender(Renderer* renderer, const Rectangle& bounds) override;

		private:
			const Color* _trackColor = nullptr;
			const Color* _activeColor = nullptr;
			const Color* _handleColor = nullptr;

			float _handlePositionFrom = 0.0f;
			float _handlePosition = 0.f;

			CallbackAnimation _animation {};
	};
}