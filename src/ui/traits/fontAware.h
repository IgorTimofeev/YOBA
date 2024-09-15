#pragma once

#include "ui/element.h"
#include "fonts/font.h"

namespace yoba {
	class FontAware : public virtual Element {
		public:
			const Font* getFont() const;
			void setFont(const Font* value);

			const Font* getFontOrDefault();

		private:
			const Font* _font = nullptr;
	};
}