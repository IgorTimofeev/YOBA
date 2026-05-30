#include "animation.h"

#include <YOBA/system.h>

namespace YOBA {
	Animation::Animation(
		const uint32_t& durationUs,
		const std::function<void(const float position)>& frameHandler
	) :
		_durationUs(durationUs),
		_frameHandler(frameHandler)
	{

	}

	Animation::Animation() : Animation(0, nullptr) {

	}

	uint32_t Animation::getDuration() const {
		return _durationUs;
	}

	void Animation::setDuration(const uint32_t& durationUs) {
		_durationUs = durationUs;
	}

	void Animation::setFrameHandler(const std::function<void(const float position)>& frameHandler) {
		_frameHandler = frameHandler;
	}

	void Animation::start() {
		_startTimeUs = system::getTimeUs();
	}

	bool Animation::isStarted() const {
		return _startTimeUs >= 0;
	}

	void Animation::stop() {
		_startTimeUs = -1;
	}

	void Animation::tick() {
		auto position = static_cast<float>(system::getTimeUs() - _startTimeUs) / static_cast<float>(_durationUs);

		if (position > 1)
			position = 1;

		if (_frameHandler)
			_frameHandler(position);

		if (position >= 1) {
			stop();
		}
	}
}