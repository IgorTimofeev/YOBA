#include "renderTarget.h"

namespace yoba {
	RenderTarget::RenderTarget(
		const Size& resolution,
		RenderingOrientation orientation,
		ColorModel colorModel,
		RenderingPixelWriting pixelWriting,
		RenderingPixelOrder pixelOrder
	) :
		_pixelWriting(pixelWriting),
		_pixelOrder(pixelOrder),
		_colorModel(colorModel),
		_defaultResolution(resolution),
		_resolution(resolution),
		_orientation(orientation)
	{

	}

	void RenderTarget::setup() {
		updateFromOrientation();
	}

	RenderingPixelWriting RenderTarget::getPixelWriting() const {
		return _pixelWriting;
	}

	RenderingPixelOrder RenderTarget::getPixelOrder() const {
		return _pixelOrder;
	}

	ColorModel RenderTarget::getColorModel() const {
		return _colorModel;
	}

	const Size& RenderTarget::getResolution() const {
		return _resolution;
	}

	RenderingOrientation RenderTarget::getOrientation() const {
		return _orientation;
	}

	void RenderTarget::setOrientation(RenderingOrientation orientation) {
		_orientation = orientation;

		onOrientationChanged();
	}

	Point RenderTarget::orientPoint(const Point& point) {
//		Serial.printf("Original position: %d x %d\n", point.getX(), point.getY());

		switch (getOrientation()) {
			case RenderingOrientation::Clockwise0:
				return {
					point.getX(),
					point.getY()
				};

			case RenderingOrientation::Clockwise90: {
				return {
					_resolution.getWidth() - point.getY(),
					point.getX()
				};
			}

			case RenderingOrientation::Clockwise180:
				return {
					_resolution.getWidth() - point.getX(),
					_resolution.getHeight() - point.getY()
				};

			default: {
				return {
					point.getY(),
					_resolution.getHeight() - point.getX()
				};
			}
		}
	}

	void RenderTarget::updateFromOrientation() {
		switch (_orientation) {
			case RenderingOrientation::Clockwise0:
			case RenderingOrientation::Clockwise180:
				_resolution = _defaultResolution;
				break;

			case RenderingOrientation::Clockwise90:
			case RenderingOrientation::Clockwise270:
				_resolution.setWidth(_defaultResolution.getHeight());
				_resolution.setHeight(_defaultResolution.getWidth());
				break;
		}
	}

	void RenderTarget::onOrientationChanged() {
		updateFromOrientation();
	}
}