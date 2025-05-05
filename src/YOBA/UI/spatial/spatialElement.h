#pragma once

#include "YOBA/main/vector3.h"
#include "YOBA/main/rendering/renderer.h"

namespace YOBA {
	class SpatialView;
	
	class SpatialElement {
		public:
			virtual ~SpatialElement() = default;

			virtual const Vector3F* getVertices() = 0;
			virtual uint16_t getVertexCount() = 0;
			virtual void onRender(Renderer* renderer, const SpatialView& spatialView, const Vector3F* vertices) = 0;
	};
}