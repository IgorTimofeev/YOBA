#pragma once

#include "element.h"
#include "hardware/screen/buffers/screenBuffer.h"
#include "bounds.h"
#include "image_source.h"

namespace yoba {
	class Image : public Element {
		public:
			void onRender(ScreenBuffer* screenBuffer) override {
				if (!getSource() || !getSource()->getPixels())
					return;

				display.renderImage(
					Bounds(
						getBounds().getPosition(),
						getSource()->getSize()
					),
					getSource()->getPixels()
				);
			}

			// -------------------------------- Getters & setters --------------------------------

			const ImageSource* getSource() const {
				return _source;
			}

			void setSource(ImageSource* source) {
				_source = source;

				invalidateRender();
			}

		private:
			ImageSource* _source = nullptr;
	};
}
