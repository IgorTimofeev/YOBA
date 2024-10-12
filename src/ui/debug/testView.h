#pragma once

#include "../../event.h"
#include "../element.h"

namespace yoba {
	class TestView : public Element {
		public:

		protected:
			void onRender(ScreenBuffer* screenBuffer) override {
				auto buffer8 = (Bit8Rgb565PaletteBuffer*) screenBuffer;
				const auto& bounds = getBounds();

				uint8_t w = 8;
				uint8_t h = 8;

				Bit8PaletteColor black = Bit8PaletteColor(0);
				Bit8PaletteColor white = Bit8PaletteColor(255);

				// Palette
				buffer8->clear(&black);

				for (int y = 0; y < buffer8->getDriver()->getResolution().getHeight(); y += h) {
					for (int x = 0; x < buffer8->getDriver()->getResolution().getWidth(); x += w) {
						buffer8->renderFilledRectangle(
							Bounds(
								x,
								y,
								w,
								h
							),
							&black
						);

						black.setIndex(black.getIndex() + 1);
					}
				}

				// Test
				buffer8->renderFilledCircle(Point(32, 32), 16, &white);
				buffer8->renderFilledRectangle(Bounds(32, 52, 80, 30), 8, &white);
			}
	};
}
