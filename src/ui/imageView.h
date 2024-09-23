#pragma once

#include "element.h"
#include "../hardware/screen/buffers/screenBuffer.h"
#include "../bounds.h"
#include "../image.h"

namespace yoba {
	class ImageView : public Element {
		public:
			ImageView() = default;

			explicit ImageView(const Image* image) {
				setImage(image);
			}

			void onRender(ScreenBuffer* screenBuffer) override {
				if (!getImage())
					return;

				screenBuffer->renderImage(
					getBounds().getPosition(),
					getImage()
				);
			}

			const Image* getImage() const {
				return _image;
			}

			void setImage(const Image* value) {
				_image = value;
				setSize(value->getSize());

				invalidateRender();
			}

		protected:
			Size getDesiredSize(ScreenBuffer* screenBuffer, const Size& availableSize) override {
				return
					getImage()
					? getImage()->getSize()
					: Size(0, 0);
			}

		private:
			const Image* _image = nullptr;
	};
}
