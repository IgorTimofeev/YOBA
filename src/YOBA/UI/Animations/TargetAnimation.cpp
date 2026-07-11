#include <cassert>

#include <YOBA/UI/Animations/TargetAnimation.hpp>

namespace YOBA {
	void TargetAnimation::start() {
		assert(!!_target && "Target couldn't be nullptr");

		Animation::start();
	}

	Element* TargetAnimation::getTarget() const {
		return _target;
	}

	void TargetAnimation::setTarget(Element* target) {
		_target = target;
	}
}