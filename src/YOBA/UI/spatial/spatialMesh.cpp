#include "spatialMesh.h"
#include "spatialView.h"

namespace YOBA {
	const Vector3F* SpatialMesh::getVertices() {
		return _vertices;
	}

	void SpatialMesh::setVertices(const Vector3F* vertices) {
		_vertices = vertices;
	}

	uint16_t SpatialMesh::getVertexCount() {
		return _vertexCount;
	}

	void SpatialMesh::setVertexCount(uint16_t vertexCount) {
		_vertexCount = vertexCount;
	}
}