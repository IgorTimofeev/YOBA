#include "screenDriver.h"

namespace yoba {
	ScreenDriver::ScreenDriver(
		ScreenPixelWritingMethod pixelWritingMethod,
		ScreenPixelAlignment pixelAlignment,
		ColorModel colorModel,
		const Size& resolution,
		ScreenOrientation orientation
	) :
		_pixelWritingMethod(pixelWritingMethod),
		_pixelAlignment(pixelAlignment),
		_colorModel(colorModel),
		_defaultResolution(resolution),
		_resolution(resolution),
		_orientation(orientation)
	{

	}

	void ScreenDriver::setup() {
		updateFromOrientation();
	}

	ScreenPixelWritingMethod ScreenDriver::getPixelWritingMethod() const {
		return _pixelWritingMethod;
	}

	ScreenPixelAlignment ScreenDriver::getPixelAlignment() const {
		return _pixelAlignment;
	}

	ColorModel ScreenDriver::getColorModel() const {
		return _colorModel;
	}

	const Size& ScreenDriver::getResolution() const {
		return _resolution;
	}

	ScreenOrientation ScreenDriver::getOrientation() const {
		return _orientation;
	}

	void ScreenDriver::setOrientation(ScreenOrientation orientation) {
		_orientation = orientation;

		onOrientationChanged();
	}

	Point ScreenDriver::orientPoint(const Point& point) {
//		Serial.printf("Original position: %d x %d\n", point.getX(), point.getY());

		switch (getOrientation()) {
			case ScreenOrientation::Clockwise0:
				return {
					point.getX(),
					point.getY()
				};

			case ScreenOrientation::Clockwise90: {
				return {
					_resolution.getWidth() - point.getY(),
					point.getX()
				};
			}

			case ScreenOrientation::Clockwise180:
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

	void ScreenDriver::updateFromOrientation() {
		switch (_orientation) {
			case ScreenOrientation::Clockwise0:
			case ScreenOrientation::Clockwise180:
				_resolution = _defaultResolution;
				break;

			case ScreenOrientation::Clockwise90:
			case ScreenOrientation::Clockwise270:
				_resolution.setWidth(_defaultResolution.getHeight());
				_resolution.setHeight(_defaultResolution.getWidth());
				break;
		}
	}

	void ScreenDriver::onOrientationChanged() {
		updateFromOrientation();
	}
}