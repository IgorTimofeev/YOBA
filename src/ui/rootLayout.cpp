#include "rootLayout.h"
#include "hardware/screen/buffers/screenBuffer.h"
#include "element.h"
#include "animation.h"

namespace yoba {
	RootLayout::RootLayout() {
		setRoot(this);
	}

	void RootLayout::onRender(ScreenBuffer* screenBuffer) {
		if (_isRendered)
			return;

		Layout::onRender(screenBuffer);
		screenBuffer->flush();

		_isRendered = true;
	}

	void RootLayout::arrange() {
		if (_isArranged)
			return;

		Layout::arrange(Bounds(getSize()));

		_isArranged = true;
	}

	void RootLayout::measure(ScreenBuffer* screenBuffer) {
		if (_isMeasured)
			return;

		Layout::measure(screenBuffer, getSize());

		_isMeasured = true;
	}

	void RootLayout::invalidateLayout() {
		_isMeasured = false;
		_isArranged = false;
	}

	void RootLayout::invalidateRender() {
		_isRendered = false;
	}

	void RootLayout::invalidate() {
		invalidateLayout();
		invalidateRender();
	}

	void RootLayout::startAnimation(Animation* animation) {
		_animations.push_back(animation);

		animation->start();
	}

	void RootLayout::animate() {
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

	void RootLayout::tick() {
		animate();

		Layout::tick();
	}

	Element *RootLayout::getCapturedElement() const {
		return _capturedElement;
	}

	void RootLayout::setCapturedElement(Element *capturedElement) {
		_capturedElement = capturedElement;
	}

	void RootLayout::handleEvent(Event &event) {
		if (getCapturedElement()) {
			getCapturedElement()->handleEvent(event);
		}
		else {
			Element::handleEvent(event);
		}
	}
}