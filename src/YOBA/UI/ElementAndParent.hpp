#pragma once

#include <cstdint>
#include <limits>
#include <concepts>

#include <YOBA/UI/Element.hpp>
#include <YOBA/UI/Parent.hpp>

namespace YOBA {
	template <typename T> requires std::derived_from<T, Parent>
	T* Element::findParent() const {
		T* requestedParent;

		auto parent = getParent();

		while (parent) {
			requestedParent = dynamic_cast<T*>(parent);

			if (requestedParent)
				return requestedParent;

			parent = parent->getParent();
		}

		return nullptr;
	}
}
