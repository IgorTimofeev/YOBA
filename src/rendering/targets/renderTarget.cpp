#include "renderTarget.h"
#include "main/bounds.h"

namespace yoba {
	RenderTarget::RenderTarget(
		const Size& size,
		PixelWriting pixelWriting,
		PixelOrder pixelOrder,
		ColorModel colorModel,
		ViewportRotation rotation
	) :
		_defaultSize(size),
		_size(size),
		_pixelWriting(pixelWriting),
		_pixelOrder(pixelOrder),
		_colorModel(colorModel),
		_orientation(rotation)
	{

	}

	void RenderTarget::setup() {
		updateFromOrientation();
	}

	PixelWriting RenderTarget::getPixelWriting() const {
		return _pixelWriting;
	}

	PixelOrder RenderTarget::getPixelOrder() const {
		return _pixelOrder;
	}

	ColorModel RenderTarget::getColorModel() const {
		return _colorModel;
	}

	const Size& RenderTarget::getSize() const {
		return _size;
	}

	ViewportRotation RenderTarget::getRotation() const {
		return _orientation;
	}

	void RenderTarget::setRotation(ViewportRotation value) {
		if (value == _orientation)
			return;

		_orientation = value;

		onOrientationChanged();
	}

	Point RenderTarget::orientPoint(const Point& point) {
//		Serial.printf("Original position: %d x %d\n", point.getX(), point.getY());

		switch (getRotation()) {
			case ViewportRotation::clockwise0:
				return {
					point.getX(),
					point.getY()
				};

			case ViewportRotation::clockwise90: {
				return {
					_size.getWidth() - point.getY(),
					point.getX()
				};
			}

			case ViewportRotation::clockwise180:
				return {
					_size.getWidth() - point.getX(),
					_size.getHeight() - point.getY()
				};

			default: {
				return {
					point.getY(),
					_size.getHeight() - point.getX()
				};
			}
		}
	}

	void RenderTarget::updateFromOrientation() {
		switch (_orientation) {
			case ViewportRotation::clockwise0:
			case ViewportRotation::clockwise180:
				_size = _defaultSize;
				break;

			case ViewportRotation::clockwise90:
			case ViewportRotation::clockwise270:
				_size.setWidth(_defaultSize.getHeight());
				_size.setHeight(_defaultSize.getWidth());
				break;
		}
	}

	void RenderTarget::onOrientationChanged() {
		updateFromOrientation();
	}

	bool RenderTarget::operator==(const RenderTarget& rhs) const {
		return
			_orientation == rhs._orientation &&
			_defaultSize == rhs._defaultSize &&
			_colorModel == rhs._colorModel &&
			_pixelOrder == rhs._pixelOrder &&
			_pixelWriting == rhs._pixelWriting;
	}

	bool RenderTarget::operator!=(const RenderTarget& rhs) const {
		return !(rhs == *this);
	}
}