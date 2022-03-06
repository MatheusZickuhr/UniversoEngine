#pragma once

namespace engine {

	enum class AttriuteType {Float, Vec2, Vec3 };

	class VertexBuffer {

	public:

		// use this contructor for a static buffer
		VertexBuffer(unsigned int vertexSize, unsigned int count, void* data);

		// use this contructor for a dynamic buffer
		VertexBuffer(unsigned int vertexSize, unsigned int count);

		VertexBuffer(const VertexBuffer& other) = delete;

		~VertexBuffer();
		
		void bind();
		
		void unbind();

		void addAttributePointer(AttriuteType attributeType, unsigned int offset);

		void pushData(void* data, unsigned int size);

	private:

		unsigned int id;
		unsigned int stride;
		unsigned int currentLocation = 0;
	};


}