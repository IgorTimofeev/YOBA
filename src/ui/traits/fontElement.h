#pragma once

#include "ui/element.h"
#include "font.h"

namespace yoba::ui {
	class FontElement : public virtual Element {
		public:
			const Font* getFont() const;
			void setFont(const Font* value);

		private:
			const Font* _font = nullptr;
	};
}