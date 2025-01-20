#include "animation.h"
#include "application.h"

namespace yoba::ui {
	Animation::Animation(Application* application, const uint32_t &duration, const std::function<void(const double &)> &interpolator) :
		_application(application),
		_duration(duration),
		_interpolator(interpolator)
	{

	}

	void Animation::start() {
		_start = system::getTime();
	}

	void Animation::stop() {
		_start = 0;
	}

	bool Animation::tick() {
		double position = (double) (system::getTime() - _start) / (double) _duration;

		if (position > 1)
			position = 1;

		_interpolator(position);

		if (position < 1) {
			return true;
		}
		else {
			completed();

			return false;
		}
	}

	bool Animation::isStarted() const {
		return _start > 0;
	}

	uint32_t Animation::getDuration() const {
		return _duration;
	}

	void Animation::setDuration(const uint32_t &duration) {
		_duration = duration;
	}

	void Animation::setInterpolator(const std::function<void(const double &)> &interpolator) {
		_interpolator = interpolator;
	}
}