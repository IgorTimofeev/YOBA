#include "renderTarget.h"
#include "YOBA/main/bounds.h"
#include "YOBA/main/rendering/renderer.h"

namespace YOBA {
	RenderTarget::RenderTarget(
		const Size& size,
		ViewportRotation rotation,
		PixelOrder pixelOrder,
		ColorModel colorModel
	) :
		_defaultSize(size),
		_size(size),
		_rotation(rotation),
		_pixelOrder(pixelOrder),
		_colorModel(colorModel)
	{
		updateSizeFromRotation();
	}

	RenderTarget::~RenderTarget() {

	}

	void RenderTarget::setup() {

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
		return _rotation;
	}

	void RenderTarget::setRotation(ViewportRotation value) {
		if (value == _rotation)
			return;

		_rotation = value;

		onRotationChanged();
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

	void RenderTarget::updateSizeFromRotation() {
		switch (_rotation) {
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

	void RenderTarget::onRotationChanged() {
		updateSizeFromRotation();

		if (_renderer)
			_renderer->updateFromTarget();
	}

	bool RenderTarget::operator==(const RenderTarget& rhs) const {
		return
			_rotation == rhs._rotation &&
			_defaultSize == rhs._defaultSize &&
			_colorModel == rhs._colorModel &&
			_pixelOrder == rhs._pixelOrder;
	}

	bool RenderTarget::operator!=(const RenderTarget& rhs) const {
		return !(rhs == *this);
	}

	Renderer* RenderTarget::getRenderer() const {
		return _renderer;
	}
}