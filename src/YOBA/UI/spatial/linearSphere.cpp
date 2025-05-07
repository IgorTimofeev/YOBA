#include "linearSphere.h"
#include "scene.h"

namespace YOBA::spatial {
	LinearSphere::LinearSphere(const Vector3F& center, float radius, const uint16_t latitudeSegments, uint16_t longitudeSegments, const Color* color) {
		const uint16_t verticesCount = 1 + latitudeSegments * longitudeSegments + 1;
		const uint16_t latitudeLinesCount = (1 + (latitudeSegments - 1) + 1) * longitudeSegments;
		const uint16_t longitudeLinesCount = latitudeSegments * longitudeSegments;

		_vertices.reserve(verticesCount);
		_lineVertexIndices.reserve((latitudeLinesCount + longitudeLinesCount) * 2);

		// Top vertex
		_vertices.push_back(center + Vector3F(0, 0, radius));

		// Middle vertices
		for (uint16_t latitudeSegment = 0; latitudeSegment < latitudeSegments; latitudeSegment++) {
			const auto latitude = static_cast<float>(latitudeSegment) / static_cast<float>(latitudeSegments) * std::numbers::pi_v<float>;

			for (uint16_t longitudeSegment = 0; longitudeSegment < longitudeSegments; longitudeSegment++) {
				const auto longitude = static_cast<float>(longitudeSegment) / static_cast<float>(longitudeSegments) * std::numbers::pi_v<float> * 2.f;

				_vertices.push_back(center + Vector3F(
					radius * std::sin(latitude) * std::cos(longitude),
					radius * std::sin(latitude) * std::sin(longitude),
					radius * std::cos(latitude)
				));

				// Latitude line

				// Top -> current
				if (latitudeSegment == 0) {
					_lineVertexIndices.push_back(0);
					_lineVertexIndices.push_back(_vertices.size() - 1);
				}
					// Middle previous -> current
				else {
					_lineVertexIndices.push_back(1 + (latitudeSegment - 1) * latitudeSegments + longitudeSegment);
					_lineVertexIndices.push_back(_vertices.size() - 1);

					// Current -> bottom (vertex will be added later)
					if (latitudeSegment == latitudeSegments - 1) {
						_lineVertexIndices.push_back(_vertices.size() - 1);
						_lineVertexIndices.push_back(verticesCount - 1);
					}
				}

				// Longitude line
				_lineVertexIndices.push_back(
					// First segment in current lat
					longitudeSegment == 0
					// Last
					? _vertices.size() - 1 + longitudeSegments - 1
					// Previous
					: _vertices.size() - 2
				);

				// Current
				_lineVertexIndices.push_back(_vertices.size() - 1);
			}
		}

		// Bottom vertex
		_vertices.push_back(center + Vector3F(0, 0, -radius));

		setVertices(_vertices.data());
		setVertexCount(_vertices.size());

		setLineVertexIndices(_lineVertexIndices.data());
		setLineVertexIndicesCount(_lineVertexIndices.size());

		setColor(color);
	}
}