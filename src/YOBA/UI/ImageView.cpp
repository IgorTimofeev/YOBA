#include <YOBA/UI/ImageView.hpp>

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

	void ImageView::onRender(Renderer* renderer, const Rectangle& bounds) {
		auto pizda  = RGB888Color(0xFFFF00).toRGB565BE();
		renderer->strokeRectangle(bounds, &pizda);

		if (!getImage())
			return;

		renderer->putImage(bounds.getPosition(), getImage());
	}
}
