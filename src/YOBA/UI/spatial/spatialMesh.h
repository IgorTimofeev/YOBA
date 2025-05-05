#pragma once

#include "spatialElement.h"

namespace YOBA {
	class SpatialMesh : public SpatialElement {
		public:
			const Vector3F* getVertices() override;

			void setVertices(const Vector3F* vertices);

			uint16_t getVertexCount() override;

			void setVertexCount(uint16_t vertexCount);

		private:
			const Vector3F* _vertices = nullptr;
			uint16_t _vertexCount = 0;
	};
}