#pragma once

#include "YOBA/main/rendering/pixelBufferRenderer.h"

namespace YOBA {
	class TransactionalPixelBufferRenderer : public virtual PixelBufferRenderer {
		public:
			uint16_t computeTransactionBufferHeight() const;
			uint16_t getTransactionBufferHeight() const;

		protected:
			uint16_t _transactionBufferHeight = 0;

			void updateFromTarget() override;
	};
}