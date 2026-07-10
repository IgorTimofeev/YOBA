#pragma once

#include <YOBA/UI/Layout.hpp>
#include <YOBA/Core/Margin.hpp>

namespace YOBA {
	class MarginLayout : public Layout {
		public:
			MarginLayout() = default;

			MarginLayout(const Margin& margin) {
				_margin = margin;
			}

			const Margin& getMargin() const {
				return _margin;
			}

			void setMargin(const Margin& margin) {
				if (margin == _margin)
					return;

				_margin = margin;

				invalidate();
			}

		protected:
			Size onMeasure(const Size& availableSize) override {
				const auto& childrenSize = Layout::onMeasure(availableSize);

				return {
					static_cast<uint16_t>(std::max<int32_t>(static_cast<int32_t>(childrenSize.getWidth()) + _margin.getLeft() + _margin.getRight(), 0)),
					static_cast<uint16_t>(std::max<int32_t>(static_cast<int32_t>(childrenSize.getHeight()) + _margin.getTop() + _margin.getBottom(), 0)),
				};
			}

			void onArrange(const Rectangle& bounds) override {
				const Rectangle boundsWithMargin {
					bounds.getX() + _margin.getLeft(),
					bounds.getY() + _margin.getTop(),
					static_cast<uint16_t>(std::max<int32_t>(static_cast<int32_t>(bounds.getWidth()) - _margin.getLeft() - _margin.getRight(), 0)),
					static_cast<uint16_t>(std::max<int32_t>(static_cast<int32_t>(bounds.getHeight()) - _margin.getTop() - _margin.getBottom(), 0))
				};

				for (const auto child : *this) {
					if (child->isVisible()) {
						child->arrange(boundsWithMargin);
					}
				}
			}

		private:
			Margin _margin {};
	};
}