#pragma once

#include <YOBA/main/rendering/pixelBufferRenderer.h>

namespace YOBA {
	class TransactionalPixelBufferRenderer : public virtual PixelBufferRenderer {
		public:
			uint16_t getTransactionViewportHeight() const;

		protected:
			uint16_t _transactionViewportHeight = 0;

			virtual uint16_t computeTransactionViewportHeight() const;
			void updateFromTarget() override;
	};
}