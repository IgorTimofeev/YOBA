#include "YOBA/main/rendering/transactionalPixelBufferRenderer.h"

namespace YOBA {
	uint16_t TransactionalPixelBufferRenderer::computeTransactionBufferHeight() const {
		return getTarget()->getSize().getHeight() / 4;
	}

	uint16_t TransactionalPixelBufferRenderer::getTransactionBufferHeight() const {
		return _transactionBufferHeight;
	}

	void TransactionalPixelBufferRenderer::updateFromTarget() {
		_transactionBufferHeight = computeTransactionBufferHeight();

		PixelBufferRenderer::updateFromTarget();
	}
}