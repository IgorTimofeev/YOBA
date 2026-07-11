#pragma once

#include <cstdint>

#include <YOBA/UI/Transforms/Transform.hpp>

namespace YOBA {
	class TranslateTransform : public Transform {
		public:
			TranslateTransform(const Point& offset);
			TranslateTransform();

			const Point& getOffset() const;
			void setOffset(const Point& value);

			Rectangle computeLayoutBoundsOnArrange(Element* element, const Rectangle& parentBounds) override;

		private:
			Point _offset;
	};
}