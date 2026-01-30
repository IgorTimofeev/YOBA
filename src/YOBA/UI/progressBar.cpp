#include <YOBA/UI/progressBar.h>

namespace YOBA {
	const Color* ProgressBar::getTrackColor() const {
		return _trackColor;
	}

	void ProgressBar::setTrackColor(const Color* value) {
		_trackColor = value;

		invalidate();
	}

	const Color* ProgressBar::getFillColor() const {
		return _fillColor;
	}

	void ProgressBar::setFillColor(const Color* value) {
		_fillColor = value;

		invalidate();
	}

	void ProgressBar::onRender(Renderer* renderer, const Bounds& bounds) {
		// Track
		if (_trackColor && getValue() < valueMax) {
			renderer->renderFilledRectangle(
				bounds,
				getCornerRadius(),
				_trackColor
			);
		}

		// Fill
		if (_fillColor && getValue() > 0) {
			renderer->renderFilledRectangle(
				Bounds(
					bounds.getX(),
					bounds.getY(),
					getValue() * bounds.getWidth() / valueMax,
					bounds.getHeight()
				),
				getCornerRadius(),
				_fillColor
			);
		}
	}
}
