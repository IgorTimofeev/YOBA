#include <YOBA/UI/Divider.hpp>

namespace YOBA {
	Divider::Divider(const Orientation orientation) {
		setOrientation(orientation);
	}

	Divider::Divider(const Orientation orientation, const Color* color): Divider(orientation) {
		setFillColor(color);
	}

	Divider::Divider(const Color* color): Divider(Orientation::horizontal, color) {

	}

	Divider::Divider(): Divider(Orientation::horizontal) {

	}

	uint8_t Divider::getLineThickness() const {
		return _lineThickness;
	}

	void Divider::setLineThickness(const uint8_t value) {
		_lineThickness = value;
	}

	Size Divider::onMeasure(const Size& availableSize) {
		return
			getOrientation() == Orientation::horizontal
				? Size(
					availableSize.getWidth(),
					_lineThickness
				)
				: Size(
					_lineThickness,
					availableSize.getHeight()
				);
	}

	void Divider::onRender(Renderer* renderer, const Rectangle& bounds) {
		const auto color = getFillColor();

		if (!color)
			return;

		if (getOrientation() == Orientation::horizontal) {
			renderer->fillRectangle(
				{
					bounds.getX(),
					bounds.getYCenter() - _lineThickness / 2,
					bounds.getWidth(),
					_lineThickness
				},
				color
			);
		}
		else {
			renderer->fillRectangle(
				{
					bounds.getXCenter() - _lineThickness / 2,
					bounds.getY(),
					_lineThickness,
					bounds.getHeight()
				},
				color
			);
		}
	}
}
