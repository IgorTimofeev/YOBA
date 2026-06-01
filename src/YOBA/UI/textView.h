#pragma once

#include <vector>

#include <YOBA/UI/control.h>
#include <YOBA/UI/traits/textElement.h>
#include <YOBA/UI/traits/textColorElement.h>
#include <YOBA/UI/traits/fontElement.h>
#include <YOBA/UI/traits/fontScaleElement.h>

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
