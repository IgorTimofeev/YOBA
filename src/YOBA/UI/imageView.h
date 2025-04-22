#pragma once

#include "YOBA/UI/element.h"
#include "YOBA/main/rendering/renderer.h"
#include "YOBA/main/bounds.h"
#include "YOBA/main/image.h"

namespace YOBA {
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
