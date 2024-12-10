#include "animation.h"
#include "Arduino.h"

namespace yoba {
	Animation::Animation() = default;

	Animation::Animation(const uint32_t &duration, const std::function<void(const double &)> &interpolator) :
		_duration(duration),
		_interpolator(interpolator)
	{

	}

	void Animation::start() {
		_start = millis();
	}

	void Animation::stop() {
		_start = 0;
	}

	bool Animation::tick() {
		double position = (double) (millis() - _start) / (double) _duration;

		if (position > 1)
			position = 1;

		_interpolator(position);

		if (position < 1) {
			return true;
		}
		else {
			_onCompleted.call();

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

	const Callback<>& Animation::getOnCompleted() const {
		return _onCompleted;
	}

	// -------------------------------- Margin --------------------------------

	MarginAnimation::MarginAnimation(const Margin &from, const Margin &to, const uint32_t &duration,
									 const std::function<void(const Margin &)> &interpolator) :
		Animation(
			duration,
			[from, to, interpolator](const double& position) {
				interpolator(from.interpolate(to, position));
			}
		)
	{

	}

	// -------------------------------- Size --------------------------------

	SizeAnimation::SizeAnimation(const Size &from, const Size &to, const uint32_t &duration,
								 const std::function<void(const Size &)> &interpolator) :
		Animation(
			duration,
			[from, to, interpolator](const double& position) {
				interpolator(from.interpolate(to, position));
			}
		)
	{

	}
}