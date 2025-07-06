#include <YOBA/main/rendering/transactionalPixelBufferRenderer.h>

namespace YOBA {
	uint16_t TransactionalPixelBufferRenderer::computeTransactionViewportHeight() const {
		return getTarget()->getSize().getHeight() / 4;
	}

	uint16_t TransactionalPixelBufferRenderer::getTransactionViewportHeight() const {
		return _transactionViewportHeight;
	}

	void TransactionalPixelBufferRenderer::updateFromTarget() {
		_transactionViewportHeight = computeTransactionViewportHeight();

		PixelBufferRenderer::updateFromTarget();
	}
}