#pragma once

#include <YOBA/UI/element.h>
#include <YOBA/UI/control.h>
#include <YOBA/UI/traits/cornerRadiusElement.h>
#include <YOBA/UI/traits/orientationElement.h>

namespace YOBA {
	class ScrollBar : public Control, public OrientationElement, public CornerRadiusElement {
		public:
			uint16_t getPosition() const;
			void setPosition(const uint16_t value);

			uint16_t getViewportSize() const;
			void setViewportSize(const uint16_t value);

			uint16_t getContentSize() const;
			void setContentSize(const uint16_t value);

			const Color* getThumbColor() const;
			void setThumbColor(const Color* value);

		protected:
			void onRender(Renderer* renderer, const Bounds& bounds) override;

		private:
			uint16_t position = 0;
			uint16_t viewportSize = 0;
			uint16_t contentSize = 0;

			const Color* thumbColor = nullptr;
	};
}