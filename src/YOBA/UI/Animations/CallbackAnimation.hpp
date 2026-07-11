#pragma once

#include <cstdint>
#include <functional>

#include <YOBA/UI/Animations/Animation.hpp>

namespace YOBA {
	class CallbackAnimation : public Animation {
		public:
			const std::function<void()>& getOnTick() const;
			void setOnTick(const std::function<void()>& callback);

		protected:
			void onTick() override;

		private:
			std::function<void()> _onTick = nullptr;
	};
}