#pragma once

#include "mesh.h"

#include "YOBA/main/color.h"

namespace YOBA::spatial {
	class LinearMesh : public Mesh {
		public:
			void onRender(Renderer* renderer, const Scene& scene, const Vector3F* vertices) override;

			const uint16_t* getLineVertexIndices() const;

			void setLineVertexIndices(const uint16_t* lineVertexIndices);

			uint16_t getLineVertexIndicesCount() const;

			void setLineVertexIndicesCount(uint16_t lineVertexIndicesCount);

			const Color* getColor() const;

			void setColor(const Color* color);

		private:
			const uint16_t* _lineVertexIndices = nullptr;
			uint16_t _lineVertexIndicesCount = 0;

			const Color* _color = nullptr;
	};
}