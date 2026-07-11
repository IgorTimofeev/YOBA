#include <YOBA/UI/Animations/Animation.hpp>
#include <YOBA/System.hpp>
#include <YOBA/UI/Application.hpp>

namespace YOBA {
	uint32_t Animation::getDuration() const {
		return _durationUs;
	}

	void Animation::setDuration(const uint32_t& durationUs) {
		_durationUs = durationUs;
	}

	void Animation::setOnStateChanged(const std::function<void(const AnimationState state)>& onStateChanged) {
		_onStateChanged = onStateChanged;
	}

	uint32_t Animation::getElapsedTime() const {
		return system::getTimeUs() - _startTimeUs;
	}

	float Animation::getProgress() const {
		return
			_startTimeUs > 0
			? static_cast<float>(std::min(getElapsedTime(), _durationUs)) / static_cast<float>(getDuration())
			: 0;
	}

	AnimationState Animation::getState() const {
		return _state;
	}

	void Animation::start() {
		if (getState() == AnimationState::started)
			return;

		const auto application = Application::getCurrent();

		if (!application)
			return;

		application->addAnimation(this);

		_startTimeUs = system::getTimeUs();
		setState(AnimationState::started);
	}

	void Animation::stop() {
		if (getState() == AnimationState::stopped)
			return;

		setState(AnimationState::stopped);
	}

	void Animation::onStateChanged(const AnimationState state) {

	}

	void Animation::tick() {
		onTick();

		// Completed
		if (getElapsedTime() >= _durationUs) {
			setState(AnimationState::completed);
		}
	}

	void Animation::setState(const AnimationState state) {
		_state = state;

		onStateChanged(_state);

		if (_onStateChanged)
			_onStateChanged(_state);
	}
}
