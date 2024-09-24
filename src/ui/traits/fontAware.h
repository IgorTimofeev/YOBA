#pragma once

#include "../element.h"
#include "../../font.h"

namespace yoba {
	class FontAware : public virtual Element {
		public:
			const Font* getFont() const;
			void setFont(const Font* value);

			const Font* getFontOrDefault();

		protected:
			const Font* _font = nullptr;

			static const Font* _defaultFont;
	};
}