#include <YOBA/UI/Spatial/SceneElement.hpp>
#include <YOBA/UI/Spatial/Scene.hpp>

namespace YOBA::spatial {
	bool SceneElement::isVisible() const {
		return _visible;
	}

	void SceneElement::setVisible(const bool value) {
		_visible = value;
	}
}