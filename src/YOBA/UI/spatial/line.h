#pragma once

#include "sceneElement.h"

#include <YOBA/main/color.h>

namespace YOBA::spatial {
	class Line : public SceneElement {
		public:
			Line(const Vector3F& from, const Vector3F& to, const Color* color);

			const Vector3F* getVertices() override;
			uint16_t getVertexCount() override;

			void onRender(Renderer* renderer, const Scene& scene, const Vector3F* projectedVertices) override;

			Vector3F getFrom() const;
			void setFrom(const Vector3F& from);

			Vector3F getTo() const;
			void setTo(const Vector3F& from);

			const Color* getColor() const;
			void setColor(const Color* color);

		private:
			Vector3F _vertices[2];

			const Color* _color = nullptr;
	};
}