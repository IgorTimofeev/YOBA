#include "renderTarget.h"

namespace yoba {
	RenderTarget::RenderTarget(
		const Size& resolution,
		RenderTargetOrientation orientation,
		ColorModel colorModel,
		RenderTargetPixelWriting pixelWriting,
		RenderTargetPixelOrder pixelOrder
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

	RenderTargetPixelWriting RenderTarget::getPixelWriting() const {
		return _pixelWriting;
	}

	RenderTargetPixelOrder RenderTarget::getPixelOrder() const {
		return _pixelOrder;
	}

	ColorModel RenderTarget::getColorModel() const {
		return _colorModel;
	}

	const Size& RenderTarget::getResolution() const {
		return _resolution;
	}

	RenderTargetOrientation RenderTarget::getOrientation() const {
		return _orientation;
	}

	void RenderTarget::setOrientation(RenderTargetOrientation orientation) {
		_orientation = orientation;

		onOrientationChanged();
	}

	Point RenderTarget::orientPoint(const Point& point) {
//		Serial.printf("Original position: %d x %d\n", point.getX(), point.getY());

		switch (getOrientation()) {
			case RenderTargetOrientation::clockwise0:
				return {
					point.getX(),
					point.getY()
				};

			case RenderTargetOrientation::clockwise90: {
				return {
					_resolution.getWidth() - point.getY(),
					point.getX()
				};
			}

			case RenderTargetOrientation::clockwise180:
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
			case RenderTargetOrientation::clockwise0:
			case RenderTargetOrientation::clockwise180:
				_resolution = _defaultResolution;
				break;

			case RenderTargetOrientation::clockwise90:
			case RenderTargetOrientation::clockwise270:
				_resolution.setWidth(_defaultResolution.getHeight());
				_resolution.setHeight(_defaultResolution.getWidth());
				break;
		}
	}

	void RenderTarget::onOrientationChanged() {
		updateFromOrientation();
	}
}