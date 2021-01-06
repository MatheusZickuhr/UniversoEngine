#include "engine.h"

class BufferLayout {

public:
	BufferLayout() {
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(0);
	}
};
