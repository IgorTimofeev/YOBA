#pragma once

#include <cstdint>
#include <functional>

namespace YOBA {
	enum class AnimationState : uint8_t {
		started,
		stopped,
		completed
	};

	class Animation {
		public:
			Animation() = default;
			virtual ~Animation() = default;

			uint32_t getDuration() const;
			void setDuration(const uint32_t& durationUs);

			AnimationState getState() const;
			void setOnStateChanged(const std::function<void(const AnimationState state)>& onStateChanged);

			uint32_t getElapsedTime() const;
			float getProgress() const;

			virtual void start();
			virtual void stop();
			void tick();

		protected:
			virtual void onStateChanged(const AnimationState state);
			virtual void onTick() = 0;

		private:
			AnimationState _state = AnimationState::stopped;
			uint32_t _durationUs = 1'000'000;
			uint64_t _startTimeUs = 0;

			std::function<void(const AnimationState state)> _onStateChanged = nullptr;

			void setState(AnimationState state);
	};
}