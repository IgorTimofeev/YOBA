#pragma once

#include "linearMesh.h"

namespace YOBA::spatial {
	class LinearSphere : public LinearMesh {
		public:
			LinearSphere(const Vector3F& center, float radius, const uint16_t latitudeSegments, uint16_t longitudeSegments, const Color& color);

		private:
			std::vector<Vector3F> _vertices {};
			std::vector<uint16_t> _lineVertexIndices {};
	};
}