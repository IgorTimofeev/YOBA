#include "bufferedRenderTarget.h"

#ifdef ESP_PLATFORM
	#include <esp_heap_caps.h>
#include <esp_log.h>

#endif

namespace yoba::hardware {
	BufferedRenderTarget::BufferedRenderTarget() { // NOLINT(*-use-equals-default)

	}

	uint8_t BufferedRenderTarget::getBufferHeight() const {
		return _bufferHeight;
	}

	void BufferedRenderTarget::flushBuffer(const Bounds& bounds, const std::function<void(uint8_t*&, size_t&)>& pixelSetter) {
		size_t pixelIndex = 0;
		uint16_t y;
		uint8_t* pixelBufferStart;
		uint8_t* pixelBufferEnd = _buffer + _bufferLength;

		for (y = 0; y < this->_size.getHeight(); y += _bufferHeight) {
			pixelBufferStart = _buffer;

			while (pixelBufferStart < pixelBufferEnd) {
				pixelSetter(pixelBufferStart, pixelIndex);
			}

			flushBuffer(Bounds(0, y, bounds.getWidth(), _bufferHeight), _bufferLength);
		}
	}

	void BufferedRenderTarget::flushBuffer(const Size& targetSize, const Bounds& invalidatedBounds, uint32_t pixelBufferIndex, ColorModel colorModel, const std::function<void(uint8_t*&, uint32_t&)>& pixelSetter) {
//		ESP_LOGI("BUffer", "-------------------------------");

		if (invalidatedBounds.haveZeroSize()) {
			ESP_LOGI("BUffer", "Invalidation bounds have zero length");

			return;
		}
		else {
			ESP_LOGI("BUffer", "Invalidation bounds: %ld x %ld x %d x %d", invalidatedBounds.getX(), invalidatedBounds.getY(), invalidatedBounds.getWidth(), invalidatedBounds.getHeight());
		}

		int32_t x;
		int32_t y = invalidatedBounds.getY();

		uint32_t pixelLineBreak = targetSize.getWidth() - invalidatedBounds.getWidth();
		uint32_t bytesPerLine = Color::getBytesPerModel(invalidatedBounds.getWidth(), colorModel);

		uint8_t* transactionBufferPtr = _buffer;
		uint8_t* transactionBufferPtrStart = transactionBufferPtr;

		uint16_t maxTransactionHeight = _bufferLength / bytesPerLine;
		maxTransactionHeight = 20;

		int32_t fromY = y;

//		ESP_LOGI("BUffer", "_bufferLength: %zu", _bufferLength);
//		ESP_LOGI("BUffer", "bytesPerLine: %lu", bytesPerLine);
//		ESP_LOGI("BUffer", "maxTransactionHeight: %d", maxTransactionHeight);

		while (y <= invalidatedBounds.getY2()) {
			for (x = 0; x < invalidatedBounds.getWidth(); x++) {
				pixelSetter(transactionBufferPtr, pixelBufferIndex);
			}

//			ESP_LOGI("BUffer", "y: %ld", y);

			y++;
			pixelBufferIndex += pixelLineBreak;

			if (y - fromY >= maxTransactionHeight) {
//				ESP_LOGI("BUffer", "Sending to ILI: %d bytes", transactionBufferPtr - transactionBufferPtrStart);

				flushBuffer(
					Bounds(
						invalidatedBounds.getX(),
						fromY,
						invalidatedBounds.getWidth(),
						y - fromY
					),
					transactionBufferPtr - transactionBufferPtrStart
				);

				fromY = y;
				transactionBufferPtr = _buffer;
				transactionBufferPtrStart = transactionBufferPtr;
			}
		}

//		if (transactionBufferPtr > fromTransactionBufferPtr) {
//			flushBuffer(
//				Bounds(
//					invalidatedBounds.getX(),
//					y,
//					invalidatedBounds.getWidth(),
//					y - fromY
//				),
//				transactionBufferPtr - fromTransactionBufferPtr
//			);
//		}
	}

	uint8_t* BufferedRenderTarget::getBuffer() const {
		return _buffer;
	}

	size_t BufferedRenderTarget::getBufferLength() const {
		return _bufferLength;
	}

	void BufferedRenderTarget::updateFromOrientation() {
		RenderTarget::updateFromOrientation();

		// Updating pixel buffer height
		_bufferHeight = getBufferHeightForOrientation();

		// Reallocating pixel buffer
		delete _buffer;

		_bufferLength = Color::getBytesPerModel(this->_size.getWidth() * _bufferHeight, _colorModel);

		#ifdef ESP_PLATFORM
			_buffer = (uint8_t*) heap_caps_malloc(_bufferLength, MALLOC_CAP_DMA);
			assert(_buffer != nullptr);
		#else
			_buffer = new uint8_t[_bufferLength];
		#endif
	}
}