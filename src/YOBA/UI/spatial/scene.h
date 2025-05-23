#pragma once

#include <YOBA/main/bounds.h>
#include <YOBA/main/rendering/renderer.h>
#include <YOBA/UI/element.h>

#include <YOBA/UI/spatial/sceneElement.h>

namespace YOBA::spatial {
	class Scene : public virtual Element {
		public:
			std::vector<SceneElement*>& getSceneElements();
			void addElement(SceneElement* element);
			void operator+=(SceneElement* element);

			const Point& getPivotOffset() const;
			void setPivotOffset(const Point& value);

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

			float getProjectionPlaneDistance() const;

			bool isFOVVertical() const;
			void setFOVVertical(bool fovVertical);

		protected:
			void onRender(Renderer* renderer, const Bounds& bounds) override;

		private:
			std::vector<SceneElement*> _elements {};

			Point _pivotOffset {};
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