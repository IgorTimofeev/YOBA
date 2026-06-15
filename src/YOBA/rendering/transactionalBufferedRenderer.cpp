#include <YOBA/rendering/transactionalBufferedRenderer.h>

namespace YOBA {
	uint16_t TransactionalBufferedRenderer::computeTransactionViewportHeight() const {
		// On ESP32 S3 max transaction length is 32 kb

		const auto& size = getTarget()->getSize();
		const auto BPM = Color::getBytesPerModel(getTarget()->getColorModel());

		// Checking for most popular resolutions
		if (size.getWidth() == 80 && size.getHeight() == 160) {
			switch (BPM) {
				case 1: return size.getHeight();
				case 2: return size.getHeight();
				default: return size.getHeight() / 2;
			}
		}
		else if (size.getWidth() == 240 && size.getHeight() == 320) {
			switch (BPM) {
				case 1: return size.getHeight() / 4;
				case 2: {
					#if defined(CONFIG_IDF_TARGET_ESP32S3)
						return size.getHeight() / 5;
					#else
						return size.getHeight() / 4;
					#endif
				}
				default: return size.getHeight() / 10;
			}
		}
		else if (size.getWidth() == 320 && size.getHeight() == 480) {
			switch (BPM) {
				case 1: return size.getHeight() / 5;
				case 2: return size.getHeight() / 10;
				default: return size.getHeight() / 15;
			}
		}
		// Something retarded
		else {
			return getTarget()->getSize().getHeight() / 10;
		}
	}

	uint16_t TransactionalBufferedRenderer::getTransactionViewportHeight() const {
		return _transactionViewportHeight;
	}

	void TransactionalBufferedRenderer::updateFromTarget() {
		_transactionViewportHeight = computeTransactionViewportHeight();

		BufferedRenderer::updateFromTarget();
	}
}