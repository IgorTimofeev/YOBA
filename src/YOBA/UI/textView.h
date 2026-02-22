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
			TextView(const Font& font, const Color& foreground);
			TextView(const std::wstring_view text);
			TextView(const Color& foreground);

			bool isWrappingEnabled() const;
			void setWrappingEnabled(const bool value);

		protected:
			Size onMeasure(const Size& availableSize) override;
			void onRender(Renderer& renderer, const Bounds& bounds) override;

		private:
			bool _wrappingEnabled = false;
			std::vector<std::wstring_view> _wrappedLines {};
	};
}
