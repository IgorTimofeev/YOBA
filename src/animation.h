#pragma once

#include <cstdint>
#include "functional"
#include "margin.h"
#include "size.h"
#include "action.h"

namespace yoba {
	class Element;

	class Animation {
		public:
			Animation();

			explicit Animation(const uint32_t& duration, const std::function<void(const double &)>& interpolator);

			void start();

			void stop();

			bool tick();

			uint32_t getDuration() const;

			void setDuration(const uint32_t &duration);

			bool isStarted() const;

			void setInterpolator(const std::function<void(const double &)> &interpolator);

			const Action<>& getOnCompleted() const;

		private:
			std::function<void(const double&)> _interpolator;
			Action<> _onCompleted;
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