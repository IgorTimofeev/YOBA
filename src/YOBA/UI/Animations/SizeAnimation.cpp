#include <cassert>

#include <YOBA/UI/Animations/SizeAnimation.hpp>

namespace YOBA {

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
}