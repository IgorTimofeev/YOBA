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

	Rectangle ScaleTransform::processLayoutBounds(Element* element, const Rectangle& layoutBounds) {
		// Simple as fuck, nothing to do here
		if (_scale.getX() == 1 && _scale.getY() == 1)
			return layoutBounds;

		const auto newWidth = static_cast<uint16_t>(static_cast<float>(layoutBounds.getWidth()) * _scale.getX());
		const auto newHeight = static_cast<uint16_t>(static_cast<float>(layoutBounds.getHeight()) * _scale.getY());

		return {
			layoutBounds.getX()
			- static_cast<int32_t>(
				// DeltaX
				static_cast<float>(static_cast<int32_t>(newWidth) - layoutBounds.getWidth())
				* _origin.getX()
			),
			layoutBounds.getY()
			- static_cast<int32_t>(
				// DeltaY
				static_cast<float>(static_cast<int32_t>(newHeight) - layoutBounds.getHeight())
				* _origin.getY()
			),
			newWidth,
			newHeight,
		};
	}
}
