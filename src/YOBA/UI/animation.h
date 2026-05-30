#pragma once

#include <cstdint>
#include <functional>

namespace YOBA {
	class Animation {
		public:
			Animation(
				const uint32_t& durationUs,
				const std::function<void(const float position)>& frameHandler
			);

			Animation();

			uint32_t getDuration() const;
			void setDuration(const uint32_t& durationUs);

			void setFrameHandler(const std::function<void(const float position)>& frameHandler);

			void start();
			bool isStarted() const;

			void stop();

			void tick();

		private:
			uint32_t _durationUs;
			std::function<void(const float position)> _frameHandler;

			int64_t _startTimeUs = -1;
	};
}