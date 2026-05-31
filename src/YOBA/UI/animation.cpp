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

	float Animation::getProgress() const {
		return static_cast<float>(std::min(getElapsedTime(), _durationUs)) / static_cast<float>(getDuration());
	}

	AnimationState Animation::getState() const {
		return _state;
	}
	Element* Animation::getTarget() const {
		return _target;
	}

	void Animation::setTarget(Element* target) {
		_target = target;
	}

	void Animation::start() {
		assert(!!_target && "Target couldn't be nullptr");

		if (getState() == AnimationState::started)
			return;

		const auto application = Application::getCurrent();

		if (!application)
			return;

		application->addAnimation(this);

		_startTimeUs = system::getTimeUs();
		setState(AnimationState::started);
	}

	void Animation::stop() {
		if (getState() == AnimationState::stopped)
			return;

		_startTimeUs = -1;
		setState(AnimationState::stopped);
	}

	void Animation::onStateChanged(const AnimationState state) {

	}

	void Animation::tick() {
		onTick();

		// Completed
		if (getElapsedTime() >= _durationUs) {
			_startTimeUs = -1;
			setState(AnimationState::completed);
		}
	}

	void Animation::setState(const AnimationState state) {
		_state = state;

		onStateChanged(_state);

		if (_onStateChanged)
			_onStateChanged(_state);
	}

	// -------------------------------- ManualAnimation --------------------------------

	const std::function<void(const float progress)>& ManualAnimation::getOnTick() const {
		return _onTick;
	}

	void ManualAnimation::setOnTick(const std::function<void(const float progress)>& frameHandler) {
		_onTick = frameHandler;
	}

	void ManualAnimation::onTick() {
		if (_onTick)
			_onTick(getProgress());
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
			case AnimationState::completed: {
				getTarget()->setSize(_to);

				break;
			}
		}
	}

	void SizeAnimation::onTick() {
		getTarget()->setSize(_computedFrom.interpolate(_computedTo, getProgress()));
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
		_transform->setScale(_from + (_to - _from) * getProgress());

		getTarget()->invalidate();
	}
}
