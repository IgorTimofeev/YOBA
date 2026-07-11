#pragma once

#include <cstdint>
#include <functional>

#include <YOBA/UI/Animations/Animation.hpp>

namespace YOBA {
	class FloatAnimation : public Animation {
		public:
			float getFrom() const;
			void setFrom(const float from);

			float getTo() const;
			void setTo(const float to);

			float getValue() const;

			const std::function<void()>& getOnTick() const;
			void setOnTick(const std::function<void()>& callback);

		protected:
			void onTick() override;

		private:
			float _from = 0;
			float _to = 1;

			std::function<void()> _onTick = nullptr;
	};
}