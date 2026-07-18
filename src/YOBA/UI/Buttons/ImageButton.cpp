#include <YOBA/UI/Buttons/ImageButton.hpp>

namespace YOBA {
	ImageButton::ImageButton(const Image* defaultImage, const Image* activeImage) : _defaultImage(defaultImage), _activeImage(activeImage) {

	}

	ImageButton::ImageButton(const Image* defaultImage) : _defaultImage(defaultImage) {

	}

	const Image* ImageButton::getDefaultImage() const {
		return _defaultImage;
	}

	void ImageButton::setDefaultImage(const Image* const defaultImage) {
		_defaultImage = defaultImage;

		invalidate();
	}

	const Image* ImageButton::getActiveImage() const {
		return _activeImage;
	}

	void ImageButton::setActiveImage(const Image* const activeImage) {
		_activeImage = activeImage;

		invalidate();
	}

	void ImageButton::onRender(Renderer* renderer, const Rectangle& bounds) {
		Button::onRender(renderer, bounds);

		const auto image = isActive() ? _activeImage : _defaultImage;

		if (!image)
			return;

		renderer->putImage(
			bounds.getCenter() - image->getSize().getCenter(),
			image
		);
	}
}
