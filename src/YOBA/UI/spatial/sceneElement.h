#pragma once

#include "YOBA/main/vector3.h"
#include "YOBA/main/rendering/renderer.h"

namespace YOBA::spatial {
	class Scene;
	
	class SceneElement {
		public:
			virtual ~SceneElement() = default;

			virtual const Vector3F* getVertices() = 0;
			virtual uint16_t getVertexCount() = 0;
			virtual void onRender(Renderer* renderer, const Scene& scene, const Vector3F* vertices) = 0;

			bool isVisible() const;
			void setVisible(bool value);

		private:
			bool _visible = true;
	};
}