#include <YOBA/rendering/renderingTarget.h>
#include <YOBA/rendering/renderer.h>
#include <YOBA/core/rectangle.h>

namespace YOBA {
	RenderingTarget::RenderingTarget(
		const Size& size,
		const Rotation rotation,
		const PixelOrder pixelOrder,
		const ColorModel colorModel
	) :
		_defaultSize(size),
		_size(size),
		_rotation(rotation),
		_pixelOrder(pixelOrder),
		_colorModel(colorModel)
	{
		updateSizeFromRotation();
	}

	RenderingTarget::~RenderingTarget() {

	}

	void RenderingTarget::setup() {

	}

	PixelOrder RenderingTarget::getPixelOrder() const {
		return _pixelOrder;
	}

	ColorModel RenderingTarget::getColorModel() const {
		return _colorModel;
	}

	const Size& RenderingTarget::getSize() const {
		return _size;
	}

	Rotation RenderingTarget::getRotation() const {
		return _rotation;
	}

	void RenderingTarget::setRotation(const Rotation value) {
		if (value == _rotation)
			return;

		_rotation = value;

		onRotationChanged();
	}

	Point RenderingTarget::applyRotation(const Point& point) const {
//		Serial.printf("Original position: %d x %d\n", point.getX(), point.getY());

		switch (getRotation()) {
			case Rotation::none:
				return point;

			case Rotation::clockwise90: {
				return {
					_size.getWidth() - point.getY(),
					point.getX()
				};
			}

			case Rotation::clockwise180:
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

	void RenderingTarget::updateSizeFromRotation() {
		switch (_rotation) {
			case Rotation::none:
			case Rotation::clockwise180:
				_size = _defaultSize;
				break;

			case Rotation::clockwise90:
			case Rotation::clockwise270:
				_size.setWidth(_defaultSize.getHeight());
				_size.setHeight(_defaultSize.getWidth());
				break;
		}
	}

	void RenderingTarget::onRotationChanged() {
		updateSizeFromRotation();

		if (_renderer)
			_renderer->updateFromTarget();
	}

	bool RenderingTarget::operator==(const RenderingTarget& rhs) const {
		return
			_rotation == rhs._rotation &&
			_defaultSize == rhs._defaultSize &&
			_colorModel == rhs._colorModel &&
			_pixelOrder == rhs._pixelOrder;
	}

	bool RenderingTarget::operator!=(const RenderingTarget& rhs) const {
		return !(rhs == *this);
	}

	Renderer* RenderingTarget::getRenderer() const {
		return _renderer;
	}
}