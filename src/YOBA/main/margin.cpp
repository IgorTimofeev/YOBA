#include "margin.h"

namespace YOBA {
	Margin Margin::zero = Margin(0);

	Margin::Margin(int32_t left, int32_t top, int32_t right, int32_t bottom) : _left(left), _top(top), _right(right), _bottom(bottom) {

	}

	Margin::Margin(int32_t horizontal, int32_t vertical) : _left(horizontal), _top(vertical), _right(horizontal), _bottom(vertical) {

	}

	Margin::Margin(int32_t uniformValue) : _left(uniformValue), _top(uniformValue), _right(uniformValue), _bottom(uniformValue) {

	}

	void Margin::setBottom(int32_t value) {
		_bottom = value;
	}

	int32_t Margin::getBottom() const {
		return _bottom;
	}

	void Margin::setRight(int32_t value) {
		_right = value;
	}

	int32_t Margin::getRight() const {
		return _right;
	}

	void Margin::setTop(int32_t value) {
		_top = value;
	}

	int32_t Margin::getTop() const {
		return _top;
	}

	void Margin::setLeft(int32_t value) {
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

	Margin Margin::interpolate(const Margin &to, const double &position) const {
		return {
			static_cast<int32_t>(getLeft() + position * (to.getLeft() - getLeft())),
			static_cast<int32_t>(getTop() + position * (to.getTop() - getTop())),
			static_cast<int32_t>(getRight() + position * (to.getRight() - getRight())),
			static_cast<int32_t>(getBottom() + position * (to.getBottom() - getBottom()))
		};
	}

	bool Margin::operator==(const Margin& rhs) const {
		return !(*this != rhs);
	}

	bool Margin::operator!=(const Margin& rhs) const {
		return _left != rhs._left || _top != rhs._top || _right != rhs._right || _bottom != rhs._bottom;
	}
}