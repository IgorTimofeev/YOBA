#include <YOBA/Core/Margin.hpp>

namespace YOBA {
	Margin Margin::zero { 0 };

	void Margin::setBottom(const uint16_t value) {
		_bottom = value;
	}

	uint16_t Margin::getBottom() const {
		return _bottom;
	}

	void Margin::setRight(const uint16_t value) {
		_right = value;
	}

	uint16_t Margin::getRight() const {
		return _right;
	}

	void Margin::setTop(const uint16_t value) {
		_top = value;
	}

	uint16_t Margin::getTop() const {
		return _top;
	}

	void Margin::setLeft(const uint16_t value) {
		_left = value;
	}

	uint16_t Margin::getLeft() const {
		return _left;
	}

	uint16_t Margin::getHorizontal() const {
		return _left + _right;
	}

	uint16_t Margin::getVertical() const {
		return _top + _bottom;
	}

	bool Margin::operator==(const Margin& rhs) const {
		return !(*this != rhs);
	}

	bool Margin::operator!=(const Margin& rhs) const {
		return _left != rhs._left || _top != rhs._top || _right != rhs._right || _bottom != rhs._bottom;
	}
}