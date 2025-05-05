#include "spatialLabel.h"
#include "spatialView.h"

namespace YOBA {
	SpatialLabel::SpatialLabel(const Vector3F& position, const Font* font, const Color* color, std::wstring_view text) : _position(position), _font(font), _color(color), _text(text) {

	}

	Vector3F* SpatialLabel::getVertices() {
		return &_position;
	}

	uint16_t SpatialLabel::getVertexCount() {
		return 1;
	}

	void SpatialLabel::onRender(Renderer* renderer, const SpatialView& spatialView, const Vector3F* vertices) {
		if (vertices[0].getZ() < spatialView.getNearPlaneDistance())
			return;

		const auto point = Point(
			(int32_t) vertices[0].getX(),
			(int32_t) vertices[0].getY()
		);

		const uint8_t radius = 3;
		const uint8_t textOffset = 5;

		renderer->renderFilledCircle(point, radius, _color);
		renderer->renderString(Point(point.getX() + radius + textOffset, point.getY() - _font->getHeight() / 2), _font, _color, _text);
	}

	const Vector3F& SpatialLabel::getPosition() const {
		return _position;
	}

	void SpatialLabel::setPosition(const Vector3F& position) {
		_position = position;
	}

	const Color* SpatialLabel::getColor() const {
		return _color;
	}

	void SpatialLabel::setColor(const Color* color) {
		_color = color;
	}

	const Font* SpatialLabel::getFont() const {
		return _font;
	}

	void SpatialLabel::setFont(const Font* font) {
		_font = font;
	}

	std::wstring_view SpatialLabel::getText() const {
		return _text;
	}

	void SpatialLabel::setText(std::wstring_view text) {
		_text = text;
	}
}