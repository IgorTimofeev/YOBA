#include "renderTarget.h"

namespace yoba {
	RenderTarget::RenderTarget(
		const Size& size,
		RenderTargetOrientation orientation,
		ColorModel colorModel,
		RenderTargetPixelWriting pixelWriting,
		RenderTargetPixelOrder pixelOrder
	) :
		_pixelWriting(pixelWriting),
		_pixelOrder(pixelOrder),
		_colorModel(colorModel),
		_defaultSize(size),
		_size(size),
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

	const Size& RenderTarget::getSize() const {
		return _size;
	}

	RenderTargetOrientation RenderTarget::getOrientation() const {
		return _orientation;
	}

	void RenderTarget::setOrientation(RenderTargetOrientation value) {
		if (value == _orientation)
			return;

		_orientation = value;

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
					_size.getWidth() - point.getY(),
					point.getX()
				};
			}

			case RenderTargetOrientation::clockwise180:
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
			case RenderTargetOrientation::clockwise0:
			case RenderTargetOrientation::clockwise180:
				_size = _defaultSize;
				break;

			case RenderTargetOrientation::clockwise90:
			case RenderTargetOrientation::clockwise270:
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