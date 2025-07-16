#include "sceneElement.h"
#include "scene.h"

namespace YOBA::spatial {
	bool SceneElement::isVisible() const {
		return _visible;
	}

	void SceneElement::setVisible(const bool value) {
		_visible = value;
	}
}