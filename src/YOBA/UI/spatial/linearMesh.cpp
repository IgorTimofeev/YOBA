#include "linearMesh.h"
#include "scene.h"

namespace YOBA::spatial {
	void LinearMesh::onRender(Renderer* renderer, const Scene& scene, const Vector3F* vertices) {
		const auto nearPlane = scene.getNearPlaneDistance();

		for (uint16_t i = 0; i < _lineVertexIndicesCount; i += 2) {
			const auto vertex0 = vertices + _lineVertexIndices[i];
			const auto vertex1 = vertices + _lineVertexIndices[i + 1];

			if (
				vertex0->getZ() < nearPlane
				|| vertex1->getZ() < nearPlane
			)
				continue;

			renderer->renderLine(
				Point(
					static_cast<int32_t>(vertex0->getX()),
					static_cast<int32_t>(vertex0->getY())
				),
				Point(
					static_cast<int32_t>(vertex1->getX()),
					static_cast<int32_t>(vertex1->getY())
				),
				_color
			);
		}
	}

	const uint16_t* LinearMesh::getLineVertexIndices() const {
		return _lineVertexIndices;
	}

	void LinearMesh::setLineVertexIndices(const uint16_t* lineVertexIndices) {
		_lineVertexIndices = lineVertexIndices;
	}

	uint16_t LinearMesh::getLineVertexIndicesCount() const {
		return _lineVertexIndicesCount;
	}

	void LinearMesh::setLineVertexIndicesCount(const uint16_t lineVertexIndicesCount) {
		_lineVertexIndicesCount = lineVertexIndicesCount;
	}

	const Color* LinearMesh::getColor() const {
		return _color;
	}

	void LinearMesh::setColor(const Color* color) {
		_color = color;
	}
}