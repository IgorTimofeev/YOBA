#pragma once

#include "linearSpatialMesh.h"

namespace YOBA {
	class SphereLinearSpatialMesh : public LinearSpatialMesh {
		public:
			SphereLinearSpatialMesh(const Vector3F& center, float radius, const uint16_t latitudeSegments, uint16_t longitudeSegments, const Color* color);

		private:
			std::vector<Vector3F> _vertices {};
			std::vector<uint16_t> _lineVertexIndices {};
	};
}