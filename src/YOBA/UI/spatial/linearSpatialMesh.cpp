#include "linearSpatialMesh.h"
#include "spatialView.h"

namespace YOBA {
	void LinearSpatialMesh::onRender(Renderer* renderer, const SpatialView& spatialView, const Vector3F* vertices) {
		const auto nearPlane = spatialView.getNearPlaneDistance();
		const Vector3F* vertex0;
		const Vector3F* vertex1;

		for (uint16_t i = 0; i < _lineVertexIndicesCount; i += 2) {
			vertex0 = &vertices[_lineVertexIndices[i]];
			vertex1 = &vertices[_lineVertexIndices[i + 1]];

			if (
				vertex0->getZ() < nearPlane
				|| vertex1->getZ() < nearPlane
				)
				continue;

			renderer->renderLine(
				Point(
					(int32_t) vertex0->getX(),
					(int32_t) vertex0->getY()
				),
				Point(
					(int32_t) vertex1->getX(),
					(int32_t) vertex1->getY()
				),
				_color
			);
		}
	}

	const uint16_t* LinearSpatialMesh::getLineVertexIndices() const {
		return _lineVertexIndices;
	}

	void LinearSpatialMesh::setLineVertexIndices(const uint16_t* lineVertexIndices) {
		_lineVertexIndices = lineVertexIndices;
	}

	uint16_t LinearSpatialMesh::getLineVertexIndicesCount() const {
		return _lineVertexIndicesCount;
	}

	void LinearSpatialMesh::setLineVertexIndicesCount(uint16_t lineVertexIndicesCount) {
		_lineVertexIndicesCount = lineVertexIndicesCount;
	}

	const Color* LinearSpatialMesh::getColor() const {
		return _color;
	}

	void LinearSpatialMesh::setColor(const Color* color) {
		_color = color;
	}
}