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
		if (state != AnimationState::started)
			return;

		Application::getCurrent()->updateLayout();

		const auto& layoutBounds = getTarget()->getLayoutBounds();

		// Computing

		// From
		_computedFrom.setWidth(
			_from.getWidth() == Size::computed
				? layoutBounds.getWidth()
				: _from.getWidth()
		);

		_computedFrom.setHeight(
			_from.getHeight() == Size::computed
				? layoutBounds.getHeight()
				: _from.getHeight()
		);

		// To
		_computedTo.setWidth(
			_to.getWidth() == Size::computed
				? layoutBounds.getWidth()
				: _to.getWidth()
		);

		_computedTo.setHeight(
			_to.getHeight() == Size::computed
				? layoutBounds.getHeight()
				: _to.getHeight()
		);
	}

	void SizeAnimation::onTick() {
		getTarget()->setSize(_computedFrom.interpolate(_computedTo, getProgress()));
	}
}
