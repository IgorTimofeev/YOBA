#include "mesh.h"
#include "scene.h"

namespace YOBA::spatial {
	const Vector3F* Mesh::getVertices() {
		return _vertices;
	}

	void Mesh::setVertices(const Vector3F* vertices) {
		_vertices = vertices;
	}

	uint16_t Mesh::getVertexCount() {
		return _vertexCount;
	}

	void Mesh::setVertexCount(uint16_t vertexCount) {
		_vertexCount = vertexCount;
	}
}