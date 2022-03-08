#pragma once
#include <stdint.h> 

namespace engine {

	enum class AttriuteType {Float, Vec2, Vec3 };

	class VertexBuffer {

	public:

		// use this contructor for a static buffer
		VertexBuffer(uint32_t vertexSize, uint32_t count, void* data);

		// use this contructor for a dynamic buffer
		VertexBuffer(uint32_t vertexSize, uint32_t count);

		VertexBuffer(const VertexBuffer& other) = delete;

		~VertexBuffer();
		
		void bind();
		
		void unbind();

		void addAttributePointer(AttriuteType attributeType, uintptr_t offset);

		void pushData(void* data, uint32_t size);

	private:

		uint32_t id, stride, currentLocation = 0;
	};


}