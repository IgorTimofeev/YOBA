#pragma once

#include <vector>

#include <YOBA/UI/Control.hpp>
#include <YOBA/UI/Traits/TextElement.hpp>
#include <YOBA/UI/Traits/TextColorElement.hpp>
#include <YOBA/UI/Traits/FontElement.hpp>
#include <YOBA/UI/Traits/FontScaleElement.hpp>

namespace YOBA {
	class TextView : public Control, public TextElement, public FontElement, public FontScaleElement, public TextColorElement {
		public:
			TextView() = default;
			TextView(const Font* font, const Color* foreground);
			TextView(const std::string_view text);
			TextView(const Color* foreground);

			bool isWrappingEnabled() const;
			void setWrappingEnabled(const bool value);

		protected:
			Size onMeasure(const Size& availableSize) override;
			void onRender(Renderer* renderer, const Rectangle& bounds) override;

		private:
			bool _wrappingEnabled = false;
			std::vector<std::string_view> _wrappedLines {};
	};
}
