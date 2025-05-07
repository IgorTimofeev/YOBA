#include "line.h"
#include "scene.h"

namespace YOBA::spatial {
	Line::Line(const Vector3F& from, const Vector3F& to, const Color* color) : _color(color) {
		_vertices[0] = from;
		_vertices[0] = to;
	}

	Vector3F* Line::getVertices() {
		return _vertices;
	}

	uint16_t Line::getVertexCount() {
		return 2;
	}

	void Line::onRender(Renderer* renderer, const Scene& scene, const Vector3F* vertices) {
		const auto nearPlane = scene.getNearPlaneDistance();

		if (
			vertices[0].getZ() < nearPlane
			|| vertices[1].getZ() < nearPlane
			)
			return;

		renderer->renderLine(
			Point(
				static_cast<int32_t>(vertices[0].getX()),
				static_cast<int32_t>(vertices[0].getY())
			),
			Point(
				static_cast<int32_t>(vertices[1].getX()),
				static_cast<int32_t>(vertices[1].getY())
			),
			_color
		);
	}

	Vector3F Line::getFrom() const {
		return _vertices[0];
	}

	void Line::setFrom(const Vector3F& from) {
		_vertices[0] = from;
	}

	Vector3F Line::getTo() const {
		return _vertices[1];
	}

	void Line::setTo(const Vector3F& from) {
		_vertices[1] = from;
	}

	const Color* Line::getColor() const {
		return _color;
	}

	void Line::setColor(const Color* color) {
		_color = color;
	}
}