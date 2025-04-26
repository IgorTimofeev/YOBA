#include "YOBA/main/rendering/transactionBufferRenderer.h"

#ifdef ESP_PLATFORM
	#include <esp_heap_caps.h>
#endif

namespace YOBA {
	uint16_t TransactionBufferRenderer::computeTransactionBufferHeight() {
		return getTarget()->getSize().getHeight() / 4;
	}

	void TransactionBufferRenderer::reallocateTransactionBuffer() {
		if (!getTarget())
			return;

		if (_transactionBuffer)
			heap_caps_free(_transactionBuffer);

		_transactionBufferHeight = computeTransactionBufferHeight();
		_transactionBufferLength = Color::getBytesPerModel(getTarget()->getSize().getWidth() * _transactionBufferHeight, getTarget()->getColorModel());

		#ifdef ESP_PLATFORM
			_transactionBuffer = (uint8_t*) heap_caps_malloc(_transactionBufferLength, MALLOC_CAP_DMA);
			assert(_transactionBuffer != nullptr);
		#else
			_transactionBuffer = new uint8_t[_bufferLength];
		#endif
	}

	void TransactionBufferRenderer::updateFromTarget() {
		Renderer::updateFromTarget();

		reallocateTransactionBuffer();
	}

	uint8_t* TransactionBufferRenderer::getTransactionBuffer() const {
		return _transactionBuffer;
	}

	size_t TransactionBufferRenderer::getTransactionBufferLength() const {
		return _transactionBufferLength;
	}

	uint16_t TransactionBufferRenderer::getTransactionBufferHeight() const {
		return _transactionBufferHeight;
	}
}