#pragma once

#include <vector>

#include <YOBA/Core/Math.hpp>
#include <YOBA/UI/Control.hpp>
#include <YOBA/Core/Rectangle.hpp>
#include <YOBA/Rendering/Renderers/Renderer.hpp>

#include <YOBA/UI/Spatial/SceneElement.hpp>

namespace YOBA::spatial {
	class Scene : public Control {
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

		protected:
			void onRender(Renderer* renderer, const Rectangle& bounds) override;

		private:
			std::vector<SceneElement*> _elements {};

			Point _pivotOffset {};
			Vector3F _worldRotation {};

			Vector3F _cameraPosition {};
			Vector3F _cameraRotation {};

			// 90 deg by default
			float _FOV = Math::halfPi;

			float _nearPlaneDistance = 0.1f;
			float _farPlaneDistance = 1000.f;
	};
}