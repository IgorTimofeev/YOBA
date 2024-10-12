#include "screenDriver.h"
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

	void ScreenDriver::setup() {
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

	Point ScreenDriver::orientPoint(const Point& point) {
//		Serial.printf("Original position: %d x %d\n", point.getX(), point.getY());

		switch (getOrientation()) {
			case ScreenOrientation::Portrait0:
				return {
					point.getX(),
					point.getY()
				};

			case ScreenOrientation::Landscape90: {
				int32_t tmp = point.getX();

				return {
					_resolution.getWidth() - point.getY(),
					tmp
				};
			}

			case ScreenOrientation::Portrait180:
				return {
					_resolution.getWidth() - point.getX(),
					_resolution.getHeight() - point.getY()
				};

			default: {
				int32_t tmp = point.getX();

				return {
					point.getY(),
					_resolution.getHeight() - tmp
				};
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