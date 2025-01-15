#pragma once

#include <cstdint>
#include "functional"
#include "margin.h"
#include "size.h"
#include "callback.h"

namespace yoba {
	class Animation {
		public:
			Animation();

			Callback<> completed;

			explicit Animation(const uint32_t& duration, const std::function<void(const double &)>& interpolator);

			void start();

			void stop();

			bool tick();

			uint32_t getDuration() const;

			void setDuration(const uint32_t &duration);

			bool isStarted() const;

			void setInterpolator(const std::function<void(const double &)> &interpolator);

		private:
			std::function<void(const double&)> _interpolator;
			uint32_t _start = 0;
			uint32_t _duration = 0;
	};

	class MarginAnimation : public Animation {
		public:
			explicit MarginAnimation(const Margin& from, const Margin& to, const uint32_t& duration, const std::function<void(const Margin&)>& interpolator);
	};

	class SizeAnimation : public Animation {
		public:
			explicit SizeAnimation(const Size& from, const Size& to, const uint32_t& duration, const std::function<void(const Size&)>& interpolator);
	};
}