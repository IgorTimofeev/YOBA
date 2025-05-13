#include "modal.h"

#include "application.h"

namespace YOBA {
	void ModalElement::show(Application* application) {
		*application += this;
	}

	void ModalElement::hide() {
		const auto parent = getParent();

		if (!parent)
			return;

		*parent -= this;
	}
}