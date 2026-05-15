#include <YOBA/main/rendering/transactionalPixelBufferRenderer.h>

namespace YOBA {
	uint16_t TransactionalPixelBufferRenderer::computeTransactionViewportHeight() const {
		#if defined(CONFIG_IDF_TARGET_ESP32S3)
			return getTarget()->getSize().getHeight() / 8;
		#else
			return getTarget()->getSize().getHeight() / 4;
		#endif
	}

	uint16_t TransactionalPixelBufferRenderer::getTransactionViewportHeight() const {
		return _transactionViewportHeight;
	}

	void TransactionalPixelBufferRenderer::updateFromTarget() {
		_transactionViewportHeight = computeTransactionViewportHeight();

		PixelBufferRenderer::updateFromTarget();
	}
}