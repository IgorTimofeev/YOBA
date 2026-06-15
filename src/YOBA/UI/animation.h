#pragma once

#include <cstdint>
#include <esp_log.h>
#include <functional>

#include <YOBA/core/size.h>
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
			float getProgress() const;

			Element* getTarget() const;
			void setTarget(Element* target);

			void start();
			void stop();
			void tick();

		protected:
			virtual void onStateChanged(const AnimationState state);
			virtual void onTick() = 0;

		private:
			AnimationState _state = AnimationState::stopped;
			uint32_t _durationUs = 0;
			int64_t _startTimeUs = -1;
			Element* _target = nullptr;

			std::function<void(const AnimationState state)> _onStateChanged = nullptr;

			void setState(AnimationState state);
	};

	class ManualAnimation : public Animation {
		public:
			const std::function<void(const float progress)>& getOnProgressChanged() const;
			void setOnProgressChanged(const std::function<void(const float progress)>& callback);

		protected:
			void onTick() override;

		private:
			std::function<void(const float progress)> _onProgressChanged = nullptr;
	};

	class SizeAnimation : public Animation {
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

	class ScaleTransformAnimation : public Animation {
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