#include "modal.h"

#include "application.h"

namespace YOBA {
	void ModalElement::show() {
		*Application::getCurrent() += this;
	}

	void ModalElement::hide() {
		*Application::getCurrent() -= this;
	}
}