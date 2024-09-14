#include "workspace.h"
#include "hardware/screen/buffers/screenBuffer.h"
#include "element.h"
#include "animation.h"

namespace yoba {
	Workspace::Workspace() {
		setWorkspace(this);
	}

	void Workspace::onRender(ScreenBuffer* screenBuffer) {
		if (_isRendered)
			return;

		Layout::onRender(screenBuffer);
		screenBuffer->flush();

		_isRendered = true;
	}

	void Workspace::arrange() {
		if (_isArranged)
			return;

		Layout::arrange(Bounds(getSize()));

		_isArranged = true;
	}

	void Workspace::measure(ScreenBuffer* screenBuffer) {
		if (_isMeasured)
			return;

		Layout::measure(screenBuffer, getSize());

		_isMeasured = true;
	}

	void Workspace::invalidateLayout() {
		_isMeasured = false;
		_isArranged = false;
	}

	void Workspace::invalidateRender() {
		_isRendered = false;
	}

	void Workspace::invalidate() {
		invalidateLayout();
		invalidateRender();
	}

	void Workspace::startAnimation(Animation* animation) {
		_animations.push_back(animation);

		animation->start();
	}

	void Workspace::animate() {
		if (_animations.empty())
			return;

		for (int i = 0; i < _animations.size(); i++) {
			if (_animations[i]->tick())
				continue;

			_animations[i]->stop();
			delete _animations[i];
			_animations.erase(_animations.begin() + i);

			i--;
		}
	}

	void Workspace::tick() {
		animate();

		Layout::tick();
	}

	Element *Workspace::getCapturedElement() const {
		return _capturedElement;
	}

	void Workspace::setCapturedElement(Element *capturedElement) {
		_capturedElement = capturedElement;
	}

	void Workspace::handleEvent(Event &event) {
		if (getCapturedElement()) {
			getCapturedElement()->handleEvent(event);
		}
		else {
			Element::handleEvent(event);
		}
	}
}