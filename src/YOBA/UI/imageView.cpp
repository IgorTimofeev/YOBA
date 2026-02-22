#include <YOBA/UI/imageView.h>

namespace YOBA {
	ImageView::ImageView(const Image& image) {
		setImage(image);
	}

	const Image& ImageView::getImage() const {
		return *_image;
	}

	void ImageView::setImage(const Image& value) {
		if (&value == _image)
			return;

		_image = &value;
		setSize(_image->getSize());

		invalidateRender();
	}

	Size ImageView::onMeasure(const Size& availableSize) {
		return
			_image
			? _image->getSize()
			: Size();
	}

	void ImageView::onRender(Renderer& renderer, const Bounds& bounds) {
		if (_image)
			renderer.renderImage(getBounds().getPosition(), *_image);
	}
}
