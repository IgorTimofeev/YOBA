#pragma once

#include "spatialElement.h"

#include "YOBA/main/color.h"

namespace YOBA {
	class SpatialLine : public SpatialElement {
		public:
			SpatialLine(const Vector3F& from, const Vector3F& to, const Color* color);

			Vector3F* getVertices() override;
			uint16_t getVertexCount() override;

			void onRender(Renderer* renderer, const SpatialView& spatialView, const Vector3F* vertices) override;

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