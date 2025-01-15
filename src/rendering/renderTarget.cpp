#include "renderTarget.h"

namespace yoba {
	RenderTarget::RenderTarget(
		RenderTargetPixelWriting pixelWriting,
		RenderTargetPixelOrder pixelOrder,
		ColorModel colorModel,
		const Size& resolution,
		RenderTargetOrientation orientation
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
			case RenderTargetOrientation::Clockwise0:
				return {
					point.getX(),
					point.getY()
				};

			case RenderTargetOrientation::Clockwise90: {
				return {
					_resolution.getWidth() - point.getY(),
					point.getX()
				};
			}

			case RenderTargetOrientation::Clockwise180:
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
			case RenderTargetOrientation::Clockwise0:
			case RenderTargetOrientation::Clockwise180:
				_resolution = _defaultResolution;
				break;

			case RenderTargetOrientation::Clockwise90:
			case RenderTargetOrientation::Clockwise270:
				_resolution.setWidth(_defaultResolution.getHeight());
				_resolution.setHeight(_defaultResolution.getWidth());
				break;
		}
	}

	void RenderTarget::onOrientationChanged() {
		updateFromOrientation();
	}
}