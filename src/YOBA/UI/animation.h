#pragma once

#include <cstdint>
#include <esp_log.h>
#include <functional>

#include <YOBA/main/size.h>
#include <YOBA/UI/element.h>

namespace YOBA {
	enum class AnimationState : uint8_t {
		stopped,
		started,
		completed
	};

	class Animation {
		// Application will handle tick()
		friend class Application;

		public:
			Animation() = default;
			virtual ~Animation() = default;

			uint32_t getDuration() const;
			void setDuration(const uint32_t& durationUs);

			AnimationState getState() const;
			void setOnStateChanged(const std::function<void(const AnimationState state)>& onStateChanged);

			virtual void start();
			void stop();

		protected:
			virtual void onStateChanged(const AnimationState state) = 0;
			virtual void onPositionChanged(const float position) = 0;

		private:
			uint32_t _durationUs = 0;
			int64_t _startTimeUs = stateToTime(AnimationState::stopped);

			std::function<void(const AnimationState state)> _onStateChanged = nullptr;

			void tick();

			constexpr static int64_t stateToTime(const AnimationState state) {
				switch (state) {
					case AnimationState::stopped: return -1;
					case AnimationState::completed: return -2;
					default: return 0;
				}
			}

			void callOnStateChanged(const AnimationState state);
	};

	class TargetAnimation : public Animation {
		public:
			Element* getTarget() const;
			void setTarget(Element* target);

			void start() override;

		private:
			Element* _target = nullptr;
	};

	class SizeAnimation : public TargetAnimation {
		public:
			Size getFrom() const;
			void setFrom(const Size& from);

			Size getTo() const;
			void setTo(const Size& to);

		protected:
			void onStateChanged(const AnimationState state) override;
			void onPositionChanged(const float position) override;

		private:
			Size _from;
			Size _to;

			Size _computedFrom {};
			Size _computedTo {};
	};
}