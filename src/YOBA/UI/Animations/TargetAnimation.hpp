#pragma once

#include <cstdint>

#include <YOBA/UI/Animations/Animation.hpp>
#include <YOBA/UI/Element.hpp>

namespace YOBA {
	class TargetAnimation : public Animation {
		public:
			void start() override;

			Element* getTarget() const;
			void setTarget(Element* target);

		private:
			Element* _target = nullptr;
	};
}