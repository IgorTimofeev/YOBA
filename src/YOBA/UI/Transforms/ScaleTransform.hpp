#pragma once

#include <cstdint>

#include <YOBA/UI/Transforms/Transform.hpp>

namespace YOBA {
	class ScaleTransform : public Transform {
		public:
			ScaleTransform(const Vector2F& scale, const Vector2F& origin = Vector2F(0.5f, 0.5f));
			ScaleTransform();

			constexpr static float computed = -1.f;

			const Vector2F& getScale() const;

			void setScale(const Vector2F& scale);
			void setScale(const float uniformScale);

			const Vector2F& getOrigin() const;
			void setOrigin(const Vector2F& origin);

		private:
			Vector2F _scale;
			Vector2F _origin;
	};
}