#pragma once

#include <cstdint>
#include <functional>

namespace YOBA {
	class Application;

	class Animation {
		public:

			Animation(Application* application, const uint32_t& duration, const std::function<void(const double &)>& interpolator);

			void start();

			void stop();

			bool tick();

			uint32_t getDuration() const;

			void setDuration(const uint32_t &duration);

			bool isStarted() const;

			void setInterpolator(const std::function<void(const double &)> &interpolator);

		private:
			Application* _application;
			uint32_t _duration = 0;
			uint64_t _start = 0;
			std::function<void()> _completed = nullptr;

			std::function<void(const double&)> _interpolator;
	};
}