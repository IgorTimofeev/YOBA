#pragma once

#include "YOBA/main/bounds.h"
#include "YOBA/main/rendering/renderer.h"
#include "YOBA/UI/element.h"

#include "YOBA/UI/spatial/element.h"
#include "YOBA/UI/spatial/camera.h"

namespace YOBA {
	class SpatialView : public virtual Element {
		public:
			Camera& getCamera();
			void addElement(SpatialElement* element);
			void operator+=(SpatialElement* element);

		protected:
			void onRender(Renderer* renderer, const Bounds& bounds) override;

		private:
			Camera _camera {};
			std::vector<SpatialElement*> _objects {};
	};
}