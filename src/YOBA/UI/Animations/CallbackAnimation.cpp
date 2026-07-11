#include <cassert>

#include <YOBA/UI/Animations/CallbackAnimation.hpp>

namespace YOBA {
	const std::function<void()>& CallbackAnimation::getOnTick() const {
		return _onTick;
	}

	void CallbackAnimation::setOnTick(const std::function<void()>& callback) {
		_onTick = callback;
	}

	void CallbackAnimation::onTick() {
		if (_onTick)
			_onTick();
	}
}