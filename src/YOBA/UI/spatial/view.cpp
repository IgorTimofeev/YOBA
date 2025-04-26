#include "view.h"

namespace YOBA {
	void SpatialView::onRender(Renderer* renderer, const Bounds& bounds) {
		const auto projectionPlaneDistance = (float) bounds.getWidth() / 2.f / std::tanf(_camera.getFOV() / 2.f);

		std::vector<Vector3F> _screenSpaceVertices {};

		for (auto object : _objects) {
			const auto vertices = object->getVertices();
			const auto verticesCount = object->getVertexCount();

			_screenSpaceVertices.reserve(verticesCount);
			_screenSpaceVertices.clear();

			for (uint32_t i = 0; i < verticesCount; i++) {
				auto vertex = vertices[i];

				// Translating vertex to camera
				vertex -= _camera.getPosition();

				// Rotating vertex around camera
				if (_camera.getRotation().getZ() != 0)
					vertex = vertex.rotateAroundZAxis(-_camera.getRotation().getZ());

				if (_camera.getRotation().getX() != 0)
					vertex = vertex.rotateAroundXAxis(-_camera.getRotation().getX());

				if (_camera.getRotation().getY() != 0)
					vertex = vertex.rotateAroundYAxis(-_camera.getRotation().getY());

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
				// x' = x * projectionPlaneZ / z
				//
				// The only thing we need to do is determine where the projection plane itself is located relative to the
				// camera. Trigonometry will help us:
				//
				// X
				// │         ╱│ } screenWidth - projection plane width
				// │       ╱  │
				// │     ╱    │        * x - original x
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
				// y' = y * screenHeight / 2 / tan(FOV / 2) / z
				//
				// In the final we need to move from world space to screen space. Since our world coordinate system is Z-up,
				// but the screen coordinate system is Y-up, they can be simply swapped:
				//
				// screenX = x * screenWidth / 2 / tan(FOV / 2) / y
				// screenY = z * screenHeight / 2 / tan(FOV / 2) / y

				_screenSpaceVertices.push_back(Vector3F(
					(float) bounds.getXCenter() + (vertex.getX() * projectionPlaneDistance / vertex.getY()),
					(float) bounds.getYCenter() - (vertex.getZ() * projectionPlaneDistance / vertex.getY()),
					vertex.getY()
				));
			}

			object->onRender(renderer, bounds, &_camera, _screenSpaceVertices.data());
		}
	}

	Camera& SpatialView::getCamera() {
		return _camera;
	}

	void SpatialView::addElement(SpatialElement* element) {
		_objects.push_back(element);
	}

	void SpatialView::operator+=(SpatialElement* element) {
		addElement(element);
	}
}