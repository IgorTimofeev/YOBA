#pragma once

#include <YOBA/Core/Vector3.hpp>
#include <YOBA/Rendering/Renderer.hpp>

namespace YOBA::spatial {
	class Scene;
	
	class SceneElement {
		public:
			virtual ~SceneElement() = default;

			virtual const Vector3F* getVertices() = 0;
			virtual uint16_t getVertexCount() = 0;
			virtual void onRender(Renderer* renderer, const Scene& scene, const Vector3F* projectedVertices) = 0;

			bool isVisible() const;
			void setVisible(bool value);

		private:
			bool _visible = true;
	};
}