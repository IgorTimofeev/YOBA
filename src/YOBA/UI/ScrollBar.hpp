#pragma once

#include <YOBA/UI/Element.hpp>
#include <YOBA/UI/Control.hpp>
#include <YOBA/UI/Traits/CornerRadiusElement.hpp>
#include <YOBA/UI/Traits/OrientationElement.hpp>

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
			void onRender(Renderer* renderer, const Rectangle& bounds) override;

		private:
			uint16_t position = 0;
			uint16_t viewportSize = 0;
			uint16_t contentSize = 0;

			const Color* thumbColor = nullptr;
	};
}