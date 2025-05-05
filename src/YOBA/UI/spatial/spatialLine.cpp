#include "spatialLine.h"
#include "spatialView.h"

namespace YOBA {
	SpatialLine::SpatialLine(const Vector3F& from, const Vector3F& to, const Color* color) : _color(color) {
		_vertices[0] = from;
		_vertices[0] = to;
	}

	Vector3F* SpatialLine::getVertices() {
		return _vertices;
	}

	uint16_t SpatialLine::getVertexCount() {
		return 2;
	}

	void SpatialLine::onRender(Renderer* renderer, const SpatialView& spatialView, const Vector3F* vertices) {
		const auto nearPlane = spatialView.getNearPlaneDistance();

		if (
			vertices[0].getZ() < nearPlane
			|| vertices[1].getZ() < nearPlane
			)
			return;

		renderer->renderLine(
			Point(
				(int32_t) vertices[0].getX(),
				(int32_t) vertices[0].getY()
			),
			Point(
				(int32_t) vertices[1].getX(),
				(int32_t) vertices[1].getY()
			),
			_color
		);
	}

	Vector3F SpatialLine::getFrom() const {
		return _vertices[0];
	}

	void SpatialLine::setFrom(const Vector3F& from) {
		_vertices[0] = from;
	}

	Vector3F SpatialLine::getTo() const {
		return _vertices[1];
	}

	void SpatialLine::setTo(const Vector3F& from) {
		_vertices[1] = from;
	}

	const Color* SpatialLine::getColor() const {
		return _color;
	}

	void SpatialLine::setColor(const Color* color) {
		_color = color;
	}
}