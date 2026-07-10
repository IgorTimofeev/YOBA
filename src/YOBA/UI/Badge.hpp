#pragma once

#include <YOBA/UI/Control.hpp>
#include <YOBA/UI/Traits/FillColorElement.hpp>
#include <YOBA/UI/Traits/CornerRadiusElement.hpp>
#include <YOBA/UI/Traits/TextColorElement.hpp>
#include <YOBA/UI/Traits/FontElement.hpp>
#include <YOBA/UI/Traits/FontScaleElement.hpp>
#include <YOBA/UI/Traits/TextElement.hpp>

namespace YOBA {
	class Badge : public Control, public CornerRadiusElement, public FillColorElement, public TextColorElement, public FontElement, public FontScaleElement, public TextElement {
		public:
			Badge() {

			}

			const Margin& getTextMargin() const {
				return _textMargin;
			}

			void setTextMargin(const Margin& textMargin) {
				_textMargin = textMargin;
			}

		protected:
			Size onMeasure(const Size& availableSize) override {
				const auto font = getFont();

				if (!font)
					return {};

				const auto textWidth = font->getWidth(getText());

				return {
					static_cast<uint16_t>(std::max<int32_t>(textWidth + _textMargin.getLeft() + _textMargin.getRight(), 0)),
					font->getLineHeight()
				};
			}

			void onRender(Renderer* renderer, const Rectangle& bounds) override {
				const auto fillColor = getFillColor();

				if (!fillColor)
					return;

				renderer->fillRectangle(bounds, getCornerRadius(), fillColor);

				const auto font = getFont();

				if (!font)
					return;

				const auto textColor = getTextColor();

				if (!textColor)
					return;

				const auto& text = getText();
				const auto textWidth = font->getWidth(getText());

				renderer->putText(
					Point(
						bounds.getXCenter() - textWidth / 2,
						bounds.getYCenter() - font->getLineHeight() / 2
					),
					font,
					textColor,
					getText(),
					getFontScale()
				);
			}

		private:
			Margin _textMargin {};
	};
}
