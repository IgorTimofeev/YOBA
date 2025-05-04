#pragma once

#include "YOBA/main/vector3.h"
#include "YOBA/UI/spatial/camera.h"

namespace YOBA {
	class SpatialElement {
		public:
			virtual ~SpatialElement() = default;

			virtual const Vector3F* getVertices() = 0;
			virtual uint16_t getVertexCount() = 0;
			virtual void onRender(Renderer* renderer, const Bounds& bounds, Camera* camera, const Vector3F* vertices) = 0;
	};

	class SpatialLine : public SpatialElement {
		public:
			SpatialLine(const Vector3F& from, const Vector3F& to, const Color* color) : _color(color) {
				_vertices[0] = from;
				_vertices[0] = to;
			}

			Vector3F* getVertices() override {
				return _vertices;
			}

			uint16_t getVertexCount() override {
				return 2;
			}

			void onRender(Renderer* renderer, const Bounds& bounds, Camera* camera, const Vector3F* vertices) override {
				const auto nearPlane = camera->getNearPlaneDistance();

				if (
					vertices[0].getZ() < nearPlane
					|| vertices[1].getZ() < nearPlane
				)
					return;

				renderer->renderLine(
					Point(
						(int32_t) vertices[0].getX(),
						(int32_t) vertices[0].getY()
					),
					Point(
						(int32_t) vertices[1].getX(),
						(int32_t) vertices[1].getY()
					),
					_color
				);
			}

			Vector3F getFrom() const {
				return _vertices[0];
			}

			void setFrom(const Vector3F& from) {
				_vertices[0] = from;
			}

			Vector3F getTo() const {
				return _vertices[1];
			}

			void setTo(const Vector3F& from) {
				_vertices[1] = from;
			}

			const Color* getColor() const {
				return _color;
			}

			void setColor(const Color* color) {
				_color = color;
			}

		private:
			Vector3F _vertices[2];
			const Color* _color = nullptr;
	};

	class SpatialLabel : public SpatialElement {
		public:
			SpatialLabel(const Vector3F& position, const Font* font, const Color* color, std::wstring_view text) : _position(position), _font(font), _color(color), _text(text) {

			}

			Vector3F* getVertices() override {
				return &_position;
			}

			uint16_t getVertexCount() override {
				return 1;
			}

			void onRender(Renderer* renderer, const Bounds& bounds, Camera* camera, const Vector3F* vertices) override {
				if (vertices[0].getZ() < camera->getNearPlaneDistance())
					return;

				const auto point = Point(
					(int32_t) vertices[0].getX(),
					(int32_t) vertices[0].getY()
				);

				const uint8_t radius = 3;
				const uint8_t textOffset = 5;
				
				renderer->renderFilledCircle(point, radius, _color);
				renderer->renderString(Point(point.getX() + radius + textOffset, point.getY() - _font->getHeight() / 2), _font, _color, _text);
			}

			const Vector3F& getPosition() const {
				return _position;
			}

			void setPosition(const Vector3F& position) {
				_position = position;
			}

			const Color* getColor() const {
				return _color;
			}

			void setColor(const Color* color) {
				_color = color;
			}

			const Font* getFont() const {
				return _font;
			}

			void setFont(const Font* font) {
				_font = font;
			}

			std::wstring_view getText() const {
				return _text;
			}

			void setText(std::wstring_view text) {
				_text = text;
			}

		private:
			Vector3F _position;
			const Font* _font = nullptr;
			const Color* _color = nullptr;
			std::wstring_view _text;
	};

	class SpatialMesh : public SpatialElement {
		public:
			const Vector3F* getVertices() override {
				return _vertices;
			}

			void setVertices(const Vector3F* vertices) {
				_vertices = vertices;
			}

			uint16_t getVertexCount() override {
				return _vertexCount;
			}

			void setVertexCount(uint16_t vertexCount) {
				_vertexCount = vertexCount;
			}

		private:
			const Vector3F* _vertices = nullptr;
			uint16_t _vertexCount = 0;
	};

	class LinearSpatialMesh : public SpatialMesh {
		public:
			void onRender(Renderer* renderer, const Bounds& bounds, Camera* camera, const Vector3F* vertices) override {
				const auto nearPlane = camera->getNearPlaneDistance();
				const Vector3F* vertex0;
				const Vector3F* vertex1;

				for (uint16_t i = 0; i < _lineVertexIndicesCount; i += 2) {
					vertex0 = &vertices[_lineVertexIndices[i]];
					vertex1 = &vertices[_lineVertexIndices[i + 1]];

					if (
						vertex0->getZ() < nearPlane
						|| vertex1->getZ() < nearPlane
					)
						continue;

					renderer->renderLine(
						Point(
							(int32_t) vertex0->getX(),
							(int32_t) vertex0->getY()
						),
						Point(
							(int32_t) vertex1->getX(),
							(int32_t) vertex1->getY()
						),
						_color
					);
				}
			}

			const uint16_t* getLineVertexIndices() const {
				return _lineVertexIndices;
			}

			void setLineVertexIndices(const uint16_t* lineVertexIndices) {
				_lineVertexIndices = lineVertexIndices;
			}

			uint16_t getLineVertexIndicesCount() const {
				return _lineVertexIndicesCount;
			}

			void setLineVertexIndicesCount(uint16_t lineVertexIndicesCount) {
				_lineVertexIndicesCount = lineVertexIndicesCount;
			}

			const Color* getColor() const {
				return _color;
			}

			void setColor(const Color* color) {
				_color = color;
			}

		private:
			const uint16_t* _lineVertexIndices = nullptr;
			uint16_t _lineVertexIndicesCount = 0;

			const Color* _color = nullptr;
	};

	// FRONT     LEFT      BACK      RIGHT     TOP       BOTTOM
	// 1######2  2######5  5######6  6######1  6######5  7######4
	// ########  ########  ########  ########  ########  ########
	// 0######3  3######4  4######7  7######0  1######2  0######3
	class CubeLinearSpatialMesh : public LinearSpatialMesh {
		public:
			CubeLinearSpatialMesh(const Vector3F& center, float size, const Color* color) {
				const auto sizeHalf = size / 2.f;

				_vertices[0] = center + Vector3F(-sizeHalf, -sizeHalf, -sizeHalf);
				_vertices[1] = center + Vector3F(-sizeHalf, sizeHalf, -sizeHalf);
				_vertices[2] = center + Vector3F(sizeHalf, sizeHalf, -sizeHalf);
				_vertices[3] = center + Vector3F(sizeHalf, -sizeHalf, -sizeHalf);
				
				_vertices[4] = center + Vector3F(sizeHalf, -sizeHalf, sizeHalf);
				_vertices[5] = center + Vector3F(sizeHalf, sizeHalf, sizeHalf);
				_vertices[6] = center + Vector3F(-sizeHalf, sizeHalf, sizeHalf);
				_vertices[7] = center + Vector3F(-sizeHalf, -sizeHalf, sizeHalf);

				setVertices(_vertices);
				setVertexCount(8);

				setLineVertexIndices(_lineVertexIndices);
				setLineVertexIndicesCount(24);

				setColor(color);
			}

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

	class SphereLinearSpatialMesh : public LinearSpatialMesh {
		public:
			SphereLinearSpatialMesh(const Vector3F& center, float radius, const uint16_t latitudeSegments, uint16_t longitudeSegments, const Color* color) {
				const uint16_t verticesCount = 1 + latitudeSegments * longitudeSegments + 1;
				const uint16_t latitudeLinesCount = (1 + (latitudeSegments - 1) + 1) * longitudeSegments;
				const uint16_t longitudeLinesCount = latitudeSegments * longitudeSegments;

				_vertices.reserve(verticesCount);
				_lineVertexIndices.reserve((latitudeLinesCount + longitudeLinesCount) * 2);

				// Top vertex
				_vertices.push_back(center + Vector3F(0, 0, radius));

				// Middle vertices
				for (uint16_t latitudeSegment = 0; latitudeSegment < latitudeSegments; latitudeSegment++) {
					const auto latitude = (float) latitudeSegment / (float) latitudeSegments * std::numbers::pi_v<float>;

					for (uint16_t longitudeSegment = 0; longitudeSegment < longitudeSegments; longitudeSegment++) {
						const auto longitude = (float) longitudeSegment / (float) longitudeSegments * std::numbers::pi_v<float> * 2.f;

						_vertices.push_back(center + Vector3F(
							radius * std::sin(latitude) * std::cos(longitude),
							radius * std::sin(latitude) * std::sin(longitude),
							radius * std::cos(latitude)
						));

						// Latitude line

						// Top -> current
						if (latitudeSegment == 0) {
							_lineVertexIndices.push_back(0);
							_lineVertexIndices.push_back(_vertices.size() - 1);
						}
						// Middle previous -> current
						else {
							_lineVertexIndices.push_back(1 + (latitudeSegment - 1) * latitudeSegments + longitudeSegment);
							_lineVertexIndices.push_back(_vertices.size() - 1);

							// Current -> bottom (vertex will be added later)
							if (latitudeSegment == latitudeSegments - 1) {
								_lineVertexIndices.push_back(_vertices.size() - 1);
								_lineVertexIndices.push_back(verticesCount - 1);
							}
						}

						// Longitude line
						_lineVertexIndices.push_back(
							// First segment in current lat
							longitudeSegment == 0
							// Last
							? _vertices.size() - 1 + longitudeSegments - 1
							// Previous
							: _vertices.size() - 2
						);

						// Current
						_lineVertexIndices.push_back(_vertices.size() - 1);
					}
				}

				// Bottom vertex
				_vertices.push_back(center + Vector3F(0, 0, -radius));

				setVertices(_vertices.data());
				setVertexCount(_vertices.size());

				setLineVertexIndices(_lineVertexIndices.data());
				setLineVertexIndicesCount(_lineVertexIndices.size());

				setColor(color);
			}

		private:
			std::vector<Vector3F> _vertices {};
			std::vector<uint16_t> _lineVertexIndices {};
	};
}