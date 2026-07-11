#include <YOBA/Core/Margin.hpp>

namespace YOBA {
	Margin Margin::zero { 0 };

	void Margin::setBottom(const int32_t value) {
		_bottom = value;
	}

	int32_t Margin::getBottom() const {
		return _bottom;
	}

	void Margin::setRight(const int32_t value) {
		_right = value;
	}

	int32_t Margin::getRight() const {
		return _right;
	}

	void Margin::setTop(const int32_t value) {
		_top = value;
	}

	int32_t Margin::getTop() const {
		return _top;
	}

	void Margin::setLeft(const int32_t value) {
		_left = value;
	}

	int32_t Margin::getLeft() const {
		return _left;
	}

	int32_t Margin::getHorizontal() const {
		return _left + _right;
	}

	int32_t Margin::getVertical() const {
		return _top + _bottom;
	}

	bool Margin::operator==(const Margin& rhs) const {
		return !(*this != rhs);
	}

	bool Margin::operator!=(const Margin& rhs) const {
		return _left != rhs._left || _top != rhs._top || _right != rhs._right || _bottom != rhs._bottom;
	}
}