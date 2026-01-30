#pragma once

#include <YOBA/UI/control.h>
#include <YOBA/UI/traits/activeElement.h>
#include <YOBA/UI/traits/cornerRadiusElement.h>

namespace YOBA {
	class Switch : public Control, public ActiveElement, public CornerRadiusElement {
		public:
			Switch() = default;
			Switch(const bool checked);

			Callback<> isActiveChanged {};

			const Color* getTrackColor() const;
			void setTrackColor(const Color* value);

			const Color* getCheckedColor() const;
			void setCheckedColor(const Color* value);

			const Color* getHandleColor() const;
			void setHandleColor(const Color* value);

		protected:
			void onRender(Renderer* renderer, const Bounds& bounds) override;
			void onEvent(Event* event) override;
			void onIsActiveChanged() override;

		private:
			const Color* _trackColor = nullptr;
			const Color* _checkedColor = nullptr;
			const Color* _handleColor = nullptr;
	};
}