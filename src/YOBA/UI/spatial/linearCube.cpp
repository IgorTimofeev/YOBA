#include "linearCube.h"
#include "scene.h"

namespace YOBA::spatial {
	LinearCube::LinearCube(const Vector3F& center, float size, const Color* color) {
		const auto sizeHalf = size / 2.f;

		_vertices[0] = center + Vector3F(-sizeHalf, -sizeHalf, -sizeHalf);
		_vertices[1] = center + Vector3F(-sizeHalf, sizeHalf, -sizeHalf);
		_vertices[2] = center + Vector3F(sizeHalf, sizeHalf, -sizeHalf);
		_vertices[3] = center + Vector3F(sizeHalf, -sizeHalf, -sizeHalf);

		_vertices[4] = center + Vector3F(sizeHalf, -sizeHalf, sizeHalf);
		_vertices[5] = center + Vector3F(sizeHalf, sizeHalf, sizeHalf);
		_vertices[6] = center + Vector3F(-sizeHalf, sizeHalf, sizeHalf);
		_vertices[7] = center + Vector3F(-sizeHalf, -sizeHalf, sizeHalf);

		setVertices(_vertices);
		setVertexCount(8);

		setLineVertexIndices(_lineVertexIndices);
		setLineVertexIndicesCount(24);

		setColor(color);
	}
}