#pragma once

#include <functional>

#include <YOBA/Core/Events/ScreenEvent.hpp>
#include <YOBA/Core/Events/PointerEvent.hpp>
#include <YOBA/UI/Shapes/OverlayShape.hpp>

namespace YOBA {
	class DialogOverlayShape : public OverlayShape {
		public:
			const std::function<void()>& getOnPointerDown() const {
				return _onPointerDown;
			}

			void setOnPointerDown(const std::function<void()>& onPointerDown) {
				_onPointerDown = onPointerDown;
			}

		protected:
			void onEvent(Event* event) override {
				if (!ScreenEvent::isScreen(event))
					return;

				if (event->is<PointerDownEvent>())
					_onPointerDown();

				event->setHandled(true);
			}

		private:
			std::function<void()> _onPointerDown = nullptr;
	};
}
