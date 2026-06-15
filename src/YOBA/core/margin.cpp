#include "margin.h"

namespace YOBA {
	Margin::Margin(const int32_t left, const int32_t top, const int32_t right, const int32_t bottom) : left(left), top(top), right(right), bottom(bottom) {

	}

	Margin::Margin(const int32_t horizontal, const int32_t vertical) : left(horizontal), top(vertical), right(horizontal), bottom(vertical) {

	}

	Margin::Margin(const int32_t uniformValue) : left(uniformValue), top(uniformValue), right(uniformValue), bottom(uniformValue) {

	}

	Margin Margin::zero { 0 };

	void Margin::setBottom(const int32_t value) {
		bottom = value;
	}

	int32_t Margin::getBottom() const {
		return bottom;
	}

	void Margin::setRight(const int32_t value) {
		right = value;
	}

	int32_t Margin::getRight() const {
		return right;
	}

	void Margin::setTop(const int32_t value) {
		top = value;
	}

	int32_t Margin::getTop() const {
		return top;
	}

	void Margin::setLeft(const int32_t value) {
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

	bool Margin::operator==(const Margin& rhs) const {
		return !(*this != rhs);
	}

	bool Margin::operator!=(const Margin& rhs) const {
		return left != rhs.left || top != rhs.top || right != rhs.right || bottom != rhs.bottom;
	}
}