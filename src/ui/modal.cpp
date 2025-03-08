#include "modal.h"

#include "application.h"

namespace yoba::ui {
	void ModalLayout::show(Application* application) {
		*application += this;
	}

	void ModalLayout::hide() {
		auto parent = getParent();

		if (!parent)
			return;

		*parent -= this;
	}
}