#pragma once

#include "spatialElement.h"

#include "YOBA/main/color.h"
#include "YOBA/main/font.h"

namespace YOBA {
	class SpatialLabel : public SpatialElement {
		public:
			SpatialLabel(const Vector3F& position, const Font* font, const Color* color, std::wstring_view text);

			Vector3F* getVertices() override;

			uint16_t getVertexCount() override;

			void onRender(Renderer* renderer, const SpatialView& spatialView, const Vector3F* vertices) override;

			const Vector3F& getPosition() const;

			void setPosition(const Vector3F& position);

			const Color* getColor() const;

			void setColor(const Color* color);

			const Font* getFont() const;

			void setFont(const Font* font);

			std::wstring_view getText() const;

			void setText(std::wstring_view text);

		private:
			Vector3F _position;
			const Font* _font = nullptr;
			const Color* _color = nullptr;
			std::wstring_view _text;
	};
}