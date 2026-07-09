#pragma once

#include <YOBA/Rendering/Renderers/BufferedRenderer.hpp>

namespace YOBA {
	class TransactionalBufferedRenderer : public virtual BufferedRenderer {
		public:
			uint16_t getTransactionViewportHeight() const;

		protected:
			uint16_t _transactionViewportHeight = 0;

			virtual uint16_t computeTransactionViewportHeight() const;
			void updateFromTarget() override;
	};
}