#include "animation.h"

#include <YOBA/system.h>
#include <YOBA/UI/application.h>

namespace YOBA {
	// -------------------------------- Animation --------------------------------

	uint32_t Animation::getDuration() const {
		return _durationUs;
	}

	void Animation::setDuration(const uint32_t& durationUs) {
		_durationUs = durationUs;
	}

	void Animation::setOnStateChanged(const std::function<void(const AnimationState state)>& onStateChanged) {
		_onStateChanged = onStateChanged;
	}

	uint32_t Animation::getElapsedTime() const {
		return system::getTimeUs() - _startTimeUs;
	}

	float Animation::getPosition() const {
		return static_cast<float>(std::min(getElapsedTime(), _durationUs)) / static_cast<float>(getDuration());
	}

	void Animation::start() {
		if (getState() == AnimationState::started)
			return;

		const auto application = Application::getCurrent();

		if (!application)
			return;

		application->addAnimation(this);

		_startTimeUs = system::getTimeUs();
		callOnStateChanged(AnimationState::started);
	}

	void Animation::stop() {
		if (getState() == AnimationState::stopped)
			return;

		_startTimeUs = stateToTime(AnimationState::stopped);
		callOnStateChanged(AnimationState::stopped);
	}

	void Animation::tick() {
		onTick();

		if (getElapsedTime() >= _durationUs) {
			_startTimeUs = stateToTime(AnimationState::completed);
			callOnStateChanged(AnimationState::completed);
		}
	}

	AnimationState Animation::getState() const {
		switch (_startTimeUs) {
			case -1: return AnimationState::stopped;
			case -2: return AnimationState::completed;
			default: return AnimationState::started;
		}
	}

	void Animation::callOnStateChanged(const AnimationState state) {
		onStateChanged(state);

		if (_onStateChanged)
			_onStateChanged(state);
	}

	// -------------------------------- TargetAnimation --------------------------------

	Element* TargetAnimation::getTarget() const {
		return _target;
	}

	void TargetAnimation::setTarget(Element* target) {
		_target = target;
	}

	void TargetAnimation::start() {
		assert(!!_target && "Target couldn't be nullptr");

		Animation::start();
	}

	// -------------------------------- SizeAnimation --------------------------------

	const Size& SizeAnimation::getFrom() const {
		return _from;
	}

	void SizeAnimation::setFrom(const Size& from) {
		_from = from;
	}

	const Size& SizeAnimation::getTo() const {
		return _to;
	}

	void SizeAnimation::setTo(const Size& to) {
		_to = to;
	}

	void SizeAnimation::onStateChanged(const AnimationState state) {
		switch (state) {
			case AnimationState::stopped: {
				getTarget()->setSize(_to);

				break;
			}
			case AnimationState::started: {
				const auto& layout = getTarget()->getLayoutBounds().getSize();

				// ESP_LOGI("anim", "from: %d, %d", _from.getWidth(), _from.getHeight());
				// ESP_LOGI("anim", "to: %d, %d", _to.getWidth(), _to.getHeight());


				// Measuring
				getTarget()->setSize( { Size::computed, Size::computed });
				getTarget()->measure({ Size::computed, Size::computed });
				const auto& measured = getTarget()->getMeasuredSize();

				// ESP_LOGI("anim", "measured: %d, %d", measured.getWidth(), measured.getHeight());

				// Computing

				// From
				_computedFrom.setWidth(
					_from.getWidth() == Size::computed
						? layout.getWidth()
						: _from.getWidth()
				);

				_computedFrom.setHeight(
					_from.getHeight() == Size::computed
						? layout.getHeight()
						: _from.getHeight()
				);

				// To
				_computedTo.setWidth(
					_to.getWidth() == Size::computed
						? measured.getWidth()
						: _to.getWidth()
				);

				_computedTo.setHeight(
					_to.getHeight() == Size::computed
						? measured.getHeight()
						: _to.getHeight()
				);

				// ESP_LOGI("anim", "_computedFrom: %d, %d", _computedFrom.getWidth(), _computedFrom.getHeight());
				// ESP_LOGI("anim", "_computedTo: %d, %d", _computedTo.getWidth(), _computedTo.getHeight());

				break;
			}
			case AnimationState::completed:
				break;
		}
	}

	void SizeAnimation::onTick() {
		getTarget()->setSize(_computedFrom.interpolate(_computedTo, getPosition()));
	}

	// -------------------------------- SizeAnimation --------------------------------

	ScaleTransform* ScaleTransformAnimation::getTransform() const {
		return _transform;
	}

	void ScaleTransformAnimation::setTransform(ScaleTransform* transform) {
		_transform = transform;
	}

	const Vector2F& ScaleTransformAnimation::getFrom() const {
		return _from;
	}

	void ScaleTransformAnimation::setFrom(const Vector2F& from) {
		_from = from;
	}

	const Vector2F& ScaleTransformAnimation::getTo() const {
		return _to;
	}

	void ScaleTransformAnimation::setTo(const Vector2F& to) {
		_to = to;
	}

	void ScaleTransformAnimation::onStateChanged(const AnimationState state) {
		switch (state) {
			case AnimationState::started: {
				assert(!!_transform && "Transform couldn't be nullptr");

				_transform->setScale(_from);
				getTarget()->invalidate();

				break;
			}
			default: {
				_transform->setScale(_to);
				getTarget()->invalidate();

				break;
			}
		}
	}

	void ScaleTransformAnimation::onTick() {
		_transform->setScale(_from + (_to - _from) * getPosition());

		getTarget()->invalidate();
	}
}
