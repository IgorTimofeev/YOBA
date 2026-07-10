#pragma once

#include <cstdint>
#include <functional>

#include <YOBA/Core/Size.hpp>
#include <YOBA/UI/Element.hpp>

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

	class TargetAnimation : public Animation {
		public:
			void start() override;

			Element* getTarget() const;
			void setTarget(Element* target);

		private:
			Element* _target = nullptr;
	};

	class ManualAnimation : public Animation {
		public:
			const std::function<void()>& getOnTick() const;
			void setOnTick(const std::function<void()>& callback);

		protected:
			void onTick() override;

		private:
			std::function<void()> _onTick = nullptr;
	};

	class FloatAnimation : public Animation {
		public:
			float getFrom() const {
				return _from;
			}

			void setFrom(const float from) {
				_from = from;
			}

			float getTo() const {
				return _to;
			}

			void setTo(const float to) {
				_to = to;
			}

			float getValue() const {
				return _from + (_to - _from) * getProgress();
			}

			const std::function<void()>& getOnTick() const {
				return _onTick;
			}

			void setOnTick(const std::function<void()>& callback) {
				_onTick = callback;
			}

		protected:
			void onTick() override {
				if (_onTick)
					_onTick();
			}

		private:
			float _from = 0;
			float _to = 1;

			std::function<void()> _onTick = nullptr;
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