#pragma once

#include "../../event.h"
#include "../element.h"

namespace yoba {
	class PaletteView : public Element {
		public:

		protected:
			void onRender(ScreenBuffer* screenBuffer) override {
				auto buffer8 = (Bits8PaletteBuffer*) screenBuffer;

				uint8_t w = 8;
				uint8_t h = 8;

				PaletteColor color = PaletteColor(0);

				buffer8->clear(&color);

				for (int y = 0; y < buffer8->getDriver()->getSize().getHeight(); y += h) {
					for (int x = 0; x < buffer8->getDriver()->getSize().getWidth(); x += w) {
						buffer8->renderFilledRectangle(
							Bounds(
								x,
								y,
								w,
								h
							),
							&color
						);
//
//						if (color.getIndex() == 255)
//							return;

						color.setIndex(color.getIndex() + 1);
					}
				}
			}
	};
}
