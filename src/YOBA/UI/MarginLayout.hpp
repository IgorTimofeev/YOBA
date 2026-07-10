#pragma once

#include <YOBA/UI/Layout.hpp>
#include <YOBA/Core/Margin.hpp>

namespace YOBA {
	class MarginLayout : public Layout {
		public:
			MarginLayout() = default;
			MarginLayout(const Margin& margin);

			const Margin& getMargin() const;
			void setMargin(const Margin& margin);

		protected:
			Size onMeasure(const Size& availableSize) override;
			void onArrange(const Rectangle& bounds) override;

		private:
			Margin _margin {};
	};
}