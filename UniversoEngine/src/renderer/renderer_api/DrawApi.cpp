#include "DrawApi.h"
#include <glad/glad.h>

namespace engine {

	void APIENTRY GLDebugMessageCallback(
		GLenum source,
		GLenum type,
		GLuint id,
		GLenum severity,
		GLsizei length,
		const GLchar* msg,
		const void* data
	);

	void DrawApi::init() {
		glEnable(GL_DEPTH_TEST);
		printf("Using OpenGL version: %s \n", glGetString(GL_VERSION));
	}

	void DrawApi::initDebugMode() {
		glEnable(GL_DEBUG_OUTPUT);
		glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
		glDebugMessageCallback(GLDebugMessageCallback, NULL);
	}

	void DrawApi::drawWithIdexes(unsigned int IndexCount) {
		glDrawElements(GL_TRIANGLES, IndexCount, GL_UNSIGNED_INT, nullptr);
	}

	void DrawApi::setViewPortSize(float width, float height) { glViewport(0, 0, width, height); }

	void DrawApi::clearColor(float r, float g, float b, float a) { glClearColor(r, g, b, a); }

	void DrawApi::clearDepthBuffer() { glClear(GL_DEPTH_BUFFER_BIT); }

	void DrawApi::clearColorBuffer() { glClear(GL_COLOR_BUFFER_BIT); }

	void DrawApi::clearDepthAndColorBuffer() { glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); }

	void DrawApi::cullFrontFace() { glCullFace(GL_FRONT); }

	void DrawApi::cullBackFace() { glCullFace(GL_BACK); }

	void APIENTRY GLDebugMessageCallback(
		GLenum source,
		GLenum type,
		GLuint id,
		GLenum severity,
		GLsizei length,
		const GLchar* msg,
		const void* data
	) {
		const char* _source;
		const char* _type;
		const char* _severity;

		switch (source) {
		case GL_DEBUG_SOURCE_API:
			_source = "API";
			break;

		case GL_DEBUG_SOURCE_WINDOW_SYSTEM:
			_source = "WINDOW SYSTEM";
			break;

		case GL_DEBUG_SOURCE_SHADER_COMPILER:
			_source = "SHADER COMPILER";
			break;

		case GL_DEBUG_SOURCE_THIRD_PARTY:
			_source = "THIRD PARTY";
			break;

		case GL_DEBUG_SOURCE_APPLICATION:
			_source = "APPLICATION";
			break;

		case GL_DEBUG_SOURCE_OTHER:
			_source = "UNKNOWN";
			break;

		default:
			_source = "UNKNOWN";
			break;
		}

		switch (type) {
		case GL_DEBUG_TYPE_ERROR:
			_type = "ERROR";
			break;

		case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR:
			_type = "DEPRECATED BEHAVIOR";
			break;

		case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:
			_type = "UDEFINED BEHAVIOR";
			break;

		case GL_DEBUG_TYPE_PORTABILITY:
			_type = "PORTABILITY";
			break;

		case GL_DEBUG_TYPE_PERFORMANCE:
			_type = "PERFORMANCE";
			break;

		case GL_DEBUG_TYPE_OTHER:
			_type = "OTHER";
			break;

		case GL_DEBUG_TYPE_MARKER:
			_type = "MARKER";
			break;

		default:
			_type = "UNKNOWN";
			break;
		}

		switch (severity) {
		case GL_DEBUG_SEVERITY_HIGH:
			_severity = "HIGH";
			break;

		case GL_DEBUG_SEVERITY_MEDIUM:
			_severity = "MEDIUM";
			break;

		case GL_DEBUG_SEVERITY_LOW:
			_severity = "LOW";
			break;

		case GL_DEBUG_SEVERITY_NOTIFICATION:
			_severity = "NOTIFICATION";
			break;

		default:
			_severity = "UNKNOWN";
			break;
		}



		printf("OpenGL error [%d]: %s of %s severity, raised from %s: %s\n",
			id, _type, _severity, _source, msg);
#ifdef _DEBUG
		__debugbreak();
#endif

	}

}

