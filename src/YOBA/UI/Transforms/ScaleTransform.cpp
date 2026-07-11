#include <YOBA/UI/Transforms/ScaleTransform.hpp>

namespace YOBA {
	ScaleTransform::ScaleTransform(const Vector2F& scale, const Vector2F& origin): _scale(scale), _origin(origin) {

	}

	ScaleTransform::ScaleTransform(): ScaleTransform(Vector2F(1.f, 1.f)) {

	}

	const Vector2F& ScaleTransform::getScale() const {
		return _scale;
	}

	void ScaleTransform::setScale(const Vector2F& scale) {
		_scale = scale;
	}

	void ScaleTransform::setScale(const float uniformScale) {
		setScale(Vector2F(uniformScale, uniformScale));
	}

	const Vector2F& ScaleTransform::getOrigin() const {
		return _origin;
	}

	void ScaleTransform::setOrigin(const Vector2F& origin) {
		_origin = origin;
	}

	Size ScaleTransform::processAvailableSizeBeforeMeasure(const Element* element, const Size& availableSize) {
		return availableSize;
	}

	Size ScaleTransform::processMeasuredSizeAfterMeasure(const Element* element, const Size& measuredSize) {
		return measuredSize;
	}

	Rectangle ScaleTransform::processLayoutBoundsOnArrange(const Element* element, const Rectangle& bounds) {
		return bounds;
	}

	Rectangle ScaleTransform::processRenderingBoundsOnArrange(const Element* element, const Rectangle& bounds) {
		// Simple as fuck, nothing to do here
		if (_scale.getX() == 1 && _scale.getY() == 1)
			return bounds;

		const auto newWidth = static_cast<uint16_t>(static_cast<float>(bounds.getWidth()) * _scale.getX());
		const auto newHeight = static_cast<uint16_t>(static_cast<float>(bounds.getHeight()) * _scale.getY());

		return {
			bounds.getX()
			- static_cast<int32_t>(
				// DeltaX
				static_cast<float>(static_cast<int32_t>(newWidth) - bounds.getWidth())
				* _origin.getX()
			),
			bounds.getY()
			- static_cast<int32_t>(
				// DeltaY
				static_cast<float>(static_cast<int32_t>(newHeight) - bounds.getHeight())
				* _origin.getY()
			),
			newWidth,
			newHeight,
		};
	}
}
