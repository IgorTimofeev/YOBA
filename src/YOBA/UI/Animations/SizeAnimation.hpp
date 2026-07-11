#pragma once

#include <cstdint>
#include <functional>

#include <YOBA/UI/Animations/TargetAnimation.hpp>
#include <YOBA/Core/Size.hpp>

namespace YOBA {
	class SizeAnimation : public TargetAnimation {
		public:
			const Size& getFrom() const;
			void setFrom(const Size& from);

			const Size& getTo() const;
			void setTo(const Size& to);

		protected:
			void onStateChanged(const AnimationState state) override;
			void onTick() override;

		private:
			Size _from {};
			Size _to {};

			Size _computedFrom {};
			Size _computedTo {};
	};
}