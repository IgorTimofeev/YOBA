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

		const auto application = Application::getCurrent();
		const auto target = getTarget();

		// Computing

		// From
		const auto& oldBounds = target->getLayoutBounds();

		_computedFrom.setWidth(
			_from.getWidth() == Size::computed
				? oldBounds.getWidth()
				: _from.getWidth()
		);

		_computedFrom.setHeight(
			_from.getHeight() == Size::computed
				? oldBounds.getHeight()
				: _from.getHeight()
		);

		// To
		target->setSize(_to);
		application->updateLayout();
		const auto& newBounds = target->getLayoutBounds();

		_computedTo.setWidth(
			_to.getWidth() == Size::computed
				? newBounds.getWidth()
				: _to.getWidth()
		);

		_computedTo.setHeight(
			_to.getHeight() == Size::computed
				? newBounds.getHeight()
				: _to.getHeight()
		);
	}

	void SizeAnimation::onTick() {
		getTarget()->setSize(_computedFrom.interpolate(_computedTo, getProgress()));
	}
}
