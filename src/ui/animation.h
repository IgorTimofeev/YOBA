#pragma once

#include <cstdint>
#include "functional"
#include "callback.h"

namespace yoba::ui {
	class Application;

	class Animation {
		public:
			Callback<> completed;

			Animation(Application* application, const uint32_t& duration, const std::function<void(const double &)>& interpolator);

			void start();

			void stop();

			bool tick();

			uint32_t getDuration() const;

			void setDuration(const uint32_t &duration);

			bool isStarted() const;

			void setInterpolator(const std::function<void(const double &)> &interpolator);

		private:
			std::function<void(const double&)> _interpolator;
			Application* _application;
			uint32_t _start = 0;
			uint32_t _duration = 0;
	};
}