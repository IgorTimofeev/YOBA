#include <cassert>

#include <YOBA/UI/Animations/ScaleTransformAnimation.hpp>

namespace YOBA {
	ScaleTransform* ScaleTransformAnimation::getTransform() const {
		return _transform;
	}

	void ScaleTransformAnimation::setTransform(ScaleTransform* transform) {
		_transform = transform;
	}

	const Vector2F& ScaleTransformAnimation::getFrom() const {
		return _from;
	}

	void ScaleTransformAnimation::setFrom(const Vector2F& from) {
		_from = from;
	}

	const Vector2F& ScaleTransformAnimation::getTo() const {
		return _to;
	}

	void ScaleTransformAnimation::setTo(const Vector2F& to) {
		_to = to;
	}

	void ScaleTransformAnimation::onTick() {
		if (_transform)
			_transform->setScale(_from + (_to - _from) * getProgress());

		getTarget()->invalidate();
	}
}