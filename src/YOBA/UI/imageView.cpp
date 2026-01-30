#include <YOBA/UI/imageView.h>

namespace YOBA {
	ImageView::ImageView(const Image* image) {
		setImage(image);
	}

	const Image* ImageView::getImage() const {
		return _image;
	}

	void ImageView::setImage(const Image* value) {
		if (value == _image)
			return;

		_image = value;
		setSize(_image->getSize());

		invalidateRender();
	}

	Size ImageView::onMeasure(const Size& availableSize) {
		return
		getImage()
			? getImage()->getSize()
			: Size(0, 0);
	}

	void ImageView::onRender(Renderer* renderer, const Bounds& bounds) {
		if (!getImage())
			return;

		renderer->renderImage(getBounds().getPosition(), getImage());
	}
}
