#include "scene.h"

namespace YOBA::spatial {
	void Scene::onRender(Renderer* renderer) {
		const auto& bounds = getBounds();
		const auto projectionPlaneDistance = getProjectionPlaneDistance();

		const SinAndCos worldRotationSinAndCos[3] = {
			SinAndCos(-_worldRotation.getX()),
			SinAndCos(-_worldRotation.getY()),
			SinAndCos(-_worldRotation.getZ())
		};

		const SinAndCos cameraRotationSinAndCos[3] = {
			SinAndCos(-_cameraRotation.getX()),
			SinAndCos(-_cameraRotation.getY()),
			SinAndCos(-_cameraRotation.getZ())
		};

		std::vector<Vector3F> _screenSpaceVertices {};

		for (const auto element : _elements) {
			if (!element->isVisible())
				continue;

			const auto vertices = element->getVertices();
			const auto verticesCount = element->getVertexCount();

			_screenSpaceVertices.reserve(verticesCount);
			_screenSpaceVertices.clear();

			for (uint32_t i = 0; i < verticesCount; i++) {
				auto vertex = vertices[i];

				// Translating vertex to camera
				vertex -= _cameraPosition;

				// World rotation
				if (_worldRotation.getZ() != 0)
					vertex = vertex.rotateAroundZAxis(worldRotationSinAndCos[2]);

				if (_worldRotation.getX() != 0)
					vertex = vertex.rotateAroundXAxis(worldRotationSinAndCos[0]);

				if (_worldRotation.getY() != 0)
					vertex = vertex.rotateAroundYAxis(worldRotationSinAndCos[1]);

				// Camera rotation
				if (_cameraRotation.getZ() != 0)
					vertex = vertex.rotateAroundZAxis(cameraRotationSinAndCos[2]);

				if (_cameraRotation.getX() != 0)
					vertex = vertex.rotateAroundXAxis(cameraRotationSinAndCos[0]);

				if (_cameraRotation.getY() != 0)
					vertex = vertex.rotateAroundYAxis(cameraRotationSinAndCos[1]);

				// First, we need to apply perspective projection. The basic concept is simple: the farther an object
				// is from the camera, the smaller it will be on the screen. For example, if a 3D penis 0.3 m high is
				// located 2 meters away from the camera, its projected height will be only 0.15 cm, i.e. 2 times smaller:
				//
				// x' = x / z
				//
				// However, this does not take into account such a thing as the camera's field of view. After all,
				// even the human eye has ~60 degrees of central vision, not to mention > 120 degrees of peripheral
				// vision. Therefore, any 3D renderer that implements a camera allows to customize this parameter.
				//
				// The concept is still simple: the closer the FOV to 180 degrees - the more things you will see
				// on screen, and the smaller the projected objects will be. The central point when object retain its
				// original dimensions will be 90 degrees. This may not seem obvious, but the dependence here is
				// sinusoidal, not linear. So wee need to figure out how exactly the field of view will affect the
				// coordinate when projected.
				//
				// To do this, we'll define the term "projection plane" - a screen located at a distance from the camera.
				// To clarify: the camera is your eyes, and the projection plane is the physical screen on which the pixels
				// are displayed. But the basic principle remains the same: the further the object is from the projection
				// plane, the smaller it will be when projected. That's it, no magic.
				//
				// x' = x * screenZ / z
				//
				// The only thing we need to do is determine where the projection plane itself is located relative to the
				// camera. Trigonometry will help us:
				//
				// X
				// │         ╱│ } screenWidth - projection plane width
				// │       ╱  │
				// │     ╱    │      * x - original x
				// │   ╱      * x' - projected x
				// │ ╱ )      │
				// ├ ─ ) ──── screenZ ───────────────────────── Z
				// │ ╲ ) FOV  │
				// │   ╲      │
				// │     ╲    │
				// │       ╲  │
				// │         ╲│
				//
				// Wee can see that
				// tan(FOV / 2) = (screenWidth / 2) / screenZ
				//
				// Then
				// screenZ = (screenWidth / 2) / tan(FOV / 2)
				//
				// So the final formula will be
				// x' = x * screenWidth / 2 / tan(FOV / 2) / z
				//
				// In the final we need to move from world space to screen space. Since our world coordinate system is Z-up,
				// but the screen coordinate system is Y-up, they can be simply swapped:
				//
				// screenX = x * screenWidth / 2 / tan(FOV / 2) / y

				_screenSpaceVertices.push_back(Vector3F(
					static_cast<float>(bounds.getXCenter()) + (vertex.getX() * projectionPlaneDistance / vertex.getY()),
					static_cast<float>(bounds.getYCenter()) - (vertex.getZ() * projectionPlaneDistance / vertex.getY()),
					vertex.getY()
				));
			}

			element->onRender(renderer, *this, _screenSpaceVertices.data());
		}
	}

	void Scene::addElement(SceneElement* element) {
		_elements.push_back(element);
	}

	void Scene::operator+=(SceneElement* element) {
		addElement(element);
	}

	const std::vector<SceneElement*>& Scene::getSceneElements() {
		return _elements;
	}

	const Vector3F& Scene::getWorldRotation() const {
		return _worldRotation;
	}

	void Scene::setWorldRotation(const Vector3F& value) {
		_worldRotation = value;
	}

	const Vector3F& Scene::getCameraPosition() const {
		return _cameraPosition;
	}

	void Scene::setCameraPosition(const Vector3F& value) {
		_cameraPosition = value;
	}

	const Vector3F& Scene::getCameraRotation() const {
		return _cameraRotation;
	}

	void Scene::setCameraRotation(const Vector3F& value) {
		_cameraRotation = value;
	}

	float Scene::getFOV() const {
		return _FOV;
	}

	void Scene::setFOV(float value) {
		_FOV = value;
	}

	float Scene::getNearPlaneDistance() const {
		return _nearPlaneDistance;
	}

	void Scene::setNearPlaneDistance(float value) {
		_nearPlaneDistance = value;
	}

	float Scene::getFarPlaneDistance() const {
		return _farPlaneDistance;
	}

	void Scene::setFarPlaneDistance(float value) {
		_farPlaneDistance = value;
	}

	bool Scene::isFOVVertical() const {
		return _FOVVertical;
	}

	void Scene::setFOVVertical(bool fovVertical) {
		_FOVVertical = fovVertical;
	}

	float Scene::getProjectionPlaneDistance() const {
		return _FOVVertical
			? static_cast<float>(getBounds().getHeight()) / 2.f / std::tanf(_FOV / 2.f)
			: static_cast<float>(getBounds().getWidth()) / 2.f / std::tanf(_FOV / 2.f);
	}
}