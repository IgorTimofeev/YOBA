#include <SPI.h>
#include "screenDriver.h"
#include "Arduino.h"
#include "point.h"

namespace yoba {
	ScreenDriver::ScreenDriver(
		const Size& resolution,
		ScreenOrientation orientation
	) :
		_defaultResolution(resolution),
		_resolution(resolution),
		_orientation(orientation)
	{

	}

	void ScreenDriver::begin() {
		updateDataFromOrientation();
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

	void ScreenDriver::rotatePointForOrientation(Point& point) {
//		Serial.printf("Original position: %d x %d\n", point.getX(), point.getY());

		switch (getOrientation()) {
			case ScreenOrientation::Portrait0:
				break;

			case ScreenOrientation::Landscape90: {
				int32_t x = point.getX();
				point.setX(point.getY());
				point.setY(_resolution.getHeight() - x);

				break;
			}

			case ScreenOrientation::Portrait180:
				point.setX(_resolution.getWidth() - point.getX());
				point.setY(_resolution.getHeight() - point.getY());

				break;

			case ScreenOrientation::Landscape270: {
				int32_t x = point.getX();
				point.setX(_resolution.getWidth() - point.getY());
				point.setY(x);

				break;
			}
		}
	}


	void ScreenDriver::updateDataFromOrientation() {
		// Updating size
		switch (_orientation) {
			case ScreenOrientation::Portrait0:
				_resolution.setWidth(_defaultResolution.getWidth());
				_resolution.setHeight(_defaultResolution.getHeight());
				break;

			case ScreenOrientation::Landscape90:
				_resolution.setWidth(_defaultResolution.getHeight());
				_resolution.setHeight(_defaultResolution.getWidth());
				break;

			case ScreenOrientation::Portrait180:
				_resolution.setWidth(_defaultResolution.getWidth());
				_resolution.setHeight(_defaultResolution.getHeight());
				break;

			case ScreenOrientation::Landscape270:
				_resolution.setWidth(_defaultResolution.getHeight());
				_resolution.setHeight(_defaultResolution.getWidth());
				break;
		}
	}

	void ScreenDriver::onOrientationChanged() {
		updateDataFromOrientation();
	}
}