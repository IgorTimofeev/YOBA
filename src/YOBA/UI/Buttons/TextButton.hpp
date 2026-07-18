#pragma once

#include <string_view>

#include <YOBA/UI/Buttons/Button.hpp>
#include <YOBA/Core/Colors.hpp>
#include <YOBA/Core/Margin.hpp>
#include <YOBA/UI/Traits/TextElement.hpp>
#include <YOBA/UI/Traits/FontElement.hpp>
#include <YOBA/UI/Traits/FontScaleElement.hpp>
#include <YOBA/Rendering/Renderers/Renderer.hpp>

namespace YOBA {
	class TextButton :
		public Button,
		public TextElement,
		public FontElement,
		public FontScaleElement
	{
		public:
			TextButton() = default;
			TextButton(const std::string_view text);

			const Color* getDefaultTextColor() const;
			void setDefaultTextColor(const Color* value);

			const Color* getActiveTextColor() const;
			void setActiveTextColor(const Color* value);

			const Margin& getTextMargin() const;
			void setTextMargin(const Margin& value);

		protected:
			void onRender(Renderer* renderer, const Rectangle& bounds) override;

		private:
			const Color* _defaultTextColor = nullptr;
			const Color* _activeTextColor = nullptr;

			Margin _textMargin {};
	};
}