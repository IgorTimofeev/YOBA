#pragma once

#include <YOBA/UI/keyboard/button.h>
#include <YOBA/UI/shapes/rectangle.h>
#include <YOBA/UI/layout.h>

namespace YOBA {
	class Keyboard;

	class KeyboardButtonsLayout : public Layout {
		public:
			explicit KeyboardButtonsLayout(Keyboard* keyboard);

		protected:
			Size onMeasure(const Size& availableSize) override;

			void onRender(Renderer* renderer, const Bounds& bounds) override;

		private:
			Keyboard* _keyboard;
	};
}