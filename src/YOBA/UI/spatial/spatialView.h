#pragma once

#include "YOBA/main/bounds.h"
#include "YOBA/main/rendering/renderer.h"
#include "YOBA/UI/element.h"

#include "YOBA/UI/spatial/spatialElement.h"

namespace YOBA {
	class SpatialView : public virtual Element {
		public:
			const std::vector<SpatialElement*>& getSpatialElements();
			void addElement(SpatialElement* element);
			void operator+=(SpatialElement* element);

			const Vector3F& getWorldRotation() const;

			void setWorldRotation(const Vector3F& value);

			const Vector3F& getCameraPosition() const;
			void setCameraPosition(const Vector3F& value);

			const Vector3F& getCameraRotation() const;
			void setCameraRotation(const Vector3F& value);

			float getFOV() const;
			void setFOV(float value);

			float getNearPlaneDistance() const;
			void setNearPlaneDistance(float value);

			float getFarPlaneDistance() const;
			void setFarPlaneDistance(float value);

			float getProjectionPlaneDistance();

			bool isFOVVertical() const;
			void setFOVVertical(bool fovVertical);

		protected:
			void onRender(Renderer* renderer, const Bounds& bounds) override;

		private:
			std::vector<SpatialElement*> _elements {};

			Vector3F _worldRotation {};

			Vector3F _cameraPosition {};
			Vector3F _cameraRotation {};

			// 90 deg by default
			float _FOV = std::numbers::pi_v<float> / 2.f;
			bool _FOVVertical = false;

			float _nearPlaneDistance = 0.1f;
			float _farPlaneDistance = 1000.f;

	};
}