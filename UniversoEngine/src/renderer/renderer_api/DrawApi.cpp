#include "DrawApi.h"
#include <glad/glad.h>
#include <format>
#include <string>
#include "../../debug/Log.h"
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
		//glEnable(GL_CULL_FACE);
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		LOG(std::format("Vendor: {}", (char*) glGetString(GL_VENDOR)));
		LOG(std::format("Renderer: {}", (char*) glGetString(GL_RENDERER)));
		LOG(std::format("OpenGL version: {}", (char*) glGetString(GL_VERSION)));
	}

	void DrawApi::initDebugMode() {
		glEnable(GL_DEBUG_OUTPUT);
		glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
		glDebugMessageCallback(GLDebugMessageCallback, NULL);
	}

	void DrawApi::drawWithIdexes(uint32_t IndexCount) {
		glDrawElements(GL_TRIANGLES, IndexCount, GL_UNSIGNED_INT, nullptr);
	}

	void DrawApi::draw(uint32_t vertexCount) {
		glDrawArrays(GL_TRIANGLES, 0, vertexCount);
	}

	void DrawApi::setViewPortSize(int32_t width, int32_t height) { glViewport(0, 0, width, height); }

	int32_t DrawApi::getViewPortWidth() {
		GLint viewport[4];

		glGetIntegerv(GL_VIEWPORT, viewport);
		return viewport[2];
	}

	int32_t DrawApi::getViewPortHeight() {
		GLint viewport[4];

		glGetIntegerv(GL_VIEWPORT, viewport);
		return viewport[3];
	}

	void DrawApi::clearColor(float r, float g, float b, float a) { glClearColor(r, g, b, a); }

	void DrawApi::clearDepthBuffer() { glClear(GL_DEPTH_BUFFER_BIT); }

	void DrawApi::clearColorBuffer() { glClear(GL_COLOR_BUFFER_BIT); }

	void DrawApi::clearDepthAndColorBuffer() { glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); }

	void DrawApi::cullFrontFace() { glCullFace(GL_FRONT); }

	void DrawApi::cullBackFace() { glCullFace(GL_BACK); }

	void DrawApi::setDepthFunctionToLessOrEqual() { glDepthFunc(GL_LEQUAL); }

	void DrawApi::setDepthFunctionToLess() { glDepthFunc(GL_LESS); }

	void DrawApi::enableDepthMask(bool enableDepthMask) { glDepthMask(enableDepthMask); }

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


		LOG(std::format("OpenGL message of type {} and severity {}: {}", _type, _severity, msg));
	
#ifdef _DEBUG
		//__debugbreak();
#endif

	}

}

