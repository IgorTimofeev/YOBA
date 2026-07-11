#pragma once

#include <cstdint>
#include <span>

#include <YOBA/UI/Transforms/Transform.hpp>

namespace YOBA {
	// Allows to apply multiple transforms at once
	class TransformGroup : public Transform {
		public:
			TransformGroup(const std::span<Transform*> transforms);
			TransformGroup();

		private:
			std::span<Transform*> _transforms;
	};
}