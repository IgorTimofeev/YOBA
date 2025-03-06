#pragma once

#include "element.h"
#include "rendering/renderers/renderer.h"
#include "main/bounds.h"
#include "main/image.h"

namespace yoba::ui {
	class ImageView : public Element {
		public:
			ImageView() = default;

			explicit ImageView(const Image* image) {
				setImage(image);
			}

			void onRender(Renderer* renderer, const Bounds& bounds) override {
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
				if (value == _image)
					return;

				_image = value;
				setSize(_image->getSize());

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
