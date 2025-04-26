#pragma once

#include "YOBA/main/rendering/renderer.h"
#include "YOBA/main/bounds.h"
#include <functional>

namespace YOBA {
	class TransactionBufferRenderer : public virtual Renderer {
		public:
			uint8_t* getTransactionBuffer() const;
			size_t getTransactionBufferLength() const;
			uint16_t getTransactionBufferHeight() const;

		protected:
			virtual uint16_t computeTransactionBufferHeightForTarget();

			void updateFromTarget() override;
			void reallocateTransactionBuffer();

		private:
			uint8_t* _transactionBuffer = nullptr;
			size_t _transactionBufferLength = 0;
			uint16_t _transactionBufferHeight = 0;
	};
}
