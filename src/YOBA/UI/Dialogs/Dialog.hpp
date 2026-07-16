#pragma once

#include <functional>

#include <YOBA/UI/Shapes/OverlayShape.hpp>

namespace YOBA {
	class DialogOverlayShape : public OverlayShape {
		public:
			const std::function<void()>& getOnPointerEvent() const;
			void setOnPointerEvent(const std::function<void()>& onPointerDown);

		protected:
			void onEvent(Event* event) override;

		private:
			std::function<void()> _onPointerEvent = nullptr;
	};
}
