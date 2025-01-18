#pragma once

#include "element.h"
#include "rendering/renderer.h"
#include "bounds.h"
#include "image.h"

namespace yoba::ui {
	class ImageView : public Element {
		public:
			ImageView() = default;

			explicit ImageView(const Image* image) {
				setImage(image);
			}

			void onRender(Renderer* renderer) override {
				if (!getImage())
					return;

				renderer->renderImage(
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
			Size onMeasure(const Size& availableSize) override {
				return
					getImage()
					? getImage()->getSize()
					: Size(0, 0);
			}

		private:
			const Image* _image = nullptr;
	};
}
