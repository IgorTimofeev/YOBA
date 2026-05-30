#pragma once

#include <cstdint>
#include <esp_log.h>
#include <functional>

#include <YOBA/main/size.h>
#include <YOBA/UI/element.h>

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
			float getPosition() const;

			virtual void start();
			void stop();
			void tick();

		protected:
			virtual void onStateChanged(const AnimationState state) = 0;
			virtual void onTick() = 0;

		private:
			uint32_t _durationUs = 0;
			int64_t _startTimeUs = stateToTime(AnimationState::stopped);

			std::function<void(const AnimationState state)> _onStateChanged = nullptr;

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
			const Size& getFrom() const;
			void setFrom(const Size& from);

			const Size& getTo() const;
			void setTo(const Size& to);

		protected:
			void onStateChanged(const AnimationState state) override;
			void onTick() override;

		private:
			Size _from {};
			Size _to {};

			Size _computedFrom {};
			Size _computedTo {};
	};

	class ScaleTransformAnimation : public TargetAnimation {
		public:
			ScaleTransform* getTransform() const;
			void setTransform(ScaleTransform* transform);

			const Vector2F& getFrom() const;
			void setFrom(const Vector2F& from);

			const Vector2F& getTo() const;
			void setTo(const Vector2F& to);

		protected:
			void onStateChanged(const AnimationState state) override;
			void onTick() override;

		private:
			ScaleTransform* _transform = nullptr;
			Vector2F _from {};
			Vector2F _to {};
	};
}