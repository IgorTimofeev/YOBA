#include "margin.h"

namespace yoba {
	Margin Margin::zero = Margin(0);

	Margin::Margin(int32_t left, int32_t top, int32_t right, int32_t bottom) : left(left), top(top), right(right), bottom(bottom) {

	}

	Margin::Margin(int32_t horizontal, int32_t vertical) : left(horizontal), top(vertical), right(horizontal), bottom(vertical) {

	}

	Margin::Margin(int32_t uniformValue) : left(uniformValue), top(uniformValue), right(uniformValue), bottom(uniformValue) {

	}

	void Margin::setBottom(int32_t value) {
		bottom = value;
	}

	int32_t Margin::getBottom() const {
		return bottom;
	}

	void Margin::setRight(int32_t value) {
		right = value;
	}

	int32_t Margin::getRight() const {
		return right;
	}

	void Margin::setTop(int32_t value) {
		top = value;
	}

	int32_t Margin::getTop() const {
		return top;
	}

	void Margin::setLeft(int32_t value) {
		left = value;
	}

	int32_t Margin::getLeft() const {
		return left;
	}

	int32_t Margin::getHorizontal() const {
		return left + right;
	}

	int32_t Margin::getVertical() const {
		return top + bottom;
	}

	Margin Margin::interpolate(const Margin &to, const double &position) const {
		return {
			(int32_t) (getLeft() + position * (to.getLeft() - getLeft())),
			(int32_t) (getTop() + position * (to.getTop() - getTop())),
			(int32_t) (getRight() + position * (to.getRight() - getRight())),
			(int32_t) (getBottom() + position * (to.getBottom() - getBottom()))
		};
	}
}