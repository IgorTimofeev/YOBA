#include <YOBA/Rendering/Targets/RenderingTarget.hpp>
#include <YOBA/Rendering/Renderers/Renderer.hpp>
#include <YOBA/Core/Rectangle.hpp>

namespace YOBA {
	void RenderingTarget::setup(
		const Size& defaultSize,
		const Rotation rotation,
		const PixelOrder pixelOrder,
		const ColorModel colorModel
	) {
		_defaultSize = defaultSize;
		_size = defaultSize;
		_rotation = rotation;
		_pixelOrder = pixelOrder;
		_colorModel = colorModel;

		updateSizeFromRotation();
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

	void RenderingTarget::setDefaultSize(const Size& size) {
		if (size == _defaultSize)
			return;

		_defaultSize = size;

		updateSizeFromRotation();

		if (_renderer)
			_renderer->updateFromTarget();

		onDefaultSizeChanged();
	}

	Rotation RenderingTarget::getRotation() const {
		return _rotation;
	}

	void RenderingTarget::setRotation(const Rotation value) {
		if (value == _rotation)
			return;

		_rotation = value;

		updateSizeFromRotation();

		if (_renderer)
			_renderer->updateFromTarget();

		onRotationChanged();
	}

	Point RenderingTarget::applyRotation(const Point& point) const {
		switch (getRotation()) {
			case Rotation::none:
				return point;

			case Rotation::clockwise90: return {
				_size.getWidth() - point.getY(),
				point.getX()
			};
			case Rotation::clockwise180: return {
				_size.getWidth() - point.getX(),
				_size.getHeight() - point.getY()
			};
			default: return {
				point.getY(),
				_size.getHeight() - point.getX()
			};
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

	void RenderingTarget::onDefaultSizeChanged() {

	}

	void RenderingTarget::onRotationChanged() {

	}

	Renderer* RenderingTarget::getRenderer() const {
		return _renderer;
	}

	bool RenderingTarget::operator==(const RenderingTarget& rhs) const {
		return
			_rotation == rhs._rotation
			&& _defaultSize == rhs._defaultSize
			&& _colorModel == rhs._colorModel
			&& _pixelOrder == rhs._pixelOrder;
	}

	bool RenderingTarget::operator!=(const RenderingTarget& rhs) const {
		return !(rhs == *this);
	}
}
