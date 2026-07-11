#include <cassert>

#include <YOBA/UI/Animations/FloatAnimation.hpp>

namespace YOBA {
	float FloatAnimation::getFrom() const {
		return _from;
	}

	void FloatAnimation::setFrom(const float from) {
		_from = from;
	}

	float FloatAnimation::getTo() const {
		return _to;
	}

	void FloatAnimation::setTo(const float to) {
		_to = to;
	}

	float FloatAnimation::getValue() const {
		return _from + (_to - _from) * getProgress();
	}

	const std::function<void()>& FloatAnimation::getOnTick() const {
		return _onTick;
	}

	void FloatAnimation::setOnTick(const std::function<void()>& callback) {
		_onTick = callback;
	}

	void FloatAnimation::onTick() {
		if (_onTick)
			_onTick();
	}
}