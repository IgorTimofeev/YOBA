#include <cassert>

#include <YOBA/UI/Animations/SizeAnimation.hpp>

#include "YOBA/UI/Application.hpp"

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
				Application::getCurrent()->updateLayout();

				const auto& oldSize = getTarget()->getLayoutBounds().getSize();

				// Measuring
				getTarget()->setSize( { Size::computed, Size::computed });
				getTarget()->measure({ Size::computed, Size::computed });
				const auto& measuredSize = getTarget()->getMeasuredSize();

				// Computing

				// From
				_computedFrom.setWidth(
					_from.getWidth() == Size::computed
						? oldSize.getWidth()
						: _from.getWidth()
				);

				_computedFrom.setHeight(
					_from.getHeight() == Size::computed
						? oldSize.getHeight()
						: _from.getHeight()
				);

				// To
				_computedTo.setWidth(
					_to.getWidth() == Size::computed
						? measuredSize.getWidth()
						: _to.getWidth()
				);

				_computedTo.setHeight(
					_to.getHeight() == Size::computed
						? measuredSize.getHeight()
						: _to.getHeight()
				);

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
