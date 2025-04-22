#pragma once

#include "YOBA/UI/element.h"
#include "YOBA/main/font.h"

namespace YOBA {
	class FontElement : public virtual Element {
		public:
			const Font* getFont() const;
			void setFont(const Font* value);

		private:
			const Font* _font = nullptr;
	};
}