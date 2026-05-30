#include "animation.h"
#include "application.h"

namespace YOBA {
	Animation::Animation(Application* application, const uint32_t &durationUs, const std::function<void(const double &)> &interpolator) :
		_application(application),
		_durationUs(durationUs),
		_interpolator(interpolator)
	{

	}

	void Animation::start() {
		_start = system::getTimeUs();
	}

	void Animation::stop() {
		_start = 0;
	}

	bool Animation::tick() {
		double position = static_cast<double>(system::getTimeUs() - _start) / static_cast<double>(_durationUs);

		if (position > 1)
			position = 1;

		_interpolator(position);

		if (position < 1) {
			return true;
		}
		else {
			if (_completed)
				_completed();

			return false;
		}
	}

	bool Animation::isStarted() const {
		return _start > 0;
	}

	uint32_t Animation::getDuration() const {
		return _durationUs;
	}

	void Animation::setDuration(const uint32_t &duration) {
		_durationUs = duration;
	}

	void Animation::setInterpolator(const std::function<void(const double &)> &interpolator) {
		_interpolator = interpolator;
	}
}