#pragma once

#include <cstdint>
#include <functional>

#include <YOBA/UI/Animations/TargetAnimation.hpp>
#include <YOBA/UI/Transforms/ScaleTransform.hpp>

namespace YOBA {
	class ScaleTransformAnimation : public TargetAnimation {
		public:
			ScaleTransform* getTransform() const;
			void setTransform(ScaleTransform* transform);

			const Vector2F& getFrom() const;
			void setFrom(const Vector2F& from);

			const Vector2F& getTo() const;
			void setTo(const Vector2F& to);

		protected:
			void onTick() override;

		private:
			ScaleTransform* _transform = nullptr;
			Vector2F _from {};
			Vector2F _to {};
	};
}