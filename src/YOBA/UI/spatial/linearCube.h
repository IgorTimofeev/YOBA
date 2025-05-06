#pragma once

#include "linearMesh.h"

namespace YOBA::spatial {
	// FRONT     LEFT      BACK      RIGHT     TOP       BOTTOM
	// 1######2  2######5  5######6  6######1  6######5  7######4
	// ########  ########  ########  ########  ########  ########
	// 0######3  3######4  4######7  7######0  1######2  0######3
	class LinearCube : public LinearMesh {
		public:
			LinearCube(const Vector3F& center, float size, const Color* color);

		private:
			Vector3F _vertices[8] {};

			constexpr static const uint16_t _lineVertexIndices[] {
				// Front
				0, 1,
				1, 2,
				2, 3,
				0, 3,

				// Back
				4,5,
				5,6,
				6,7,
				4,7,

				// Left
				2, 5,
				3, 4,

				// Right
				1, 6,
				0, 7
			};
	};
}