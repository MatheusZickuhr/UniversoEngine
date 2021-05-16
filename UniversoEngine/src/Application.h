#pragma once

#include "input/Input.h"
#include "scene/Scene.h"
#include "debug/Assert.h"
#include "renderer/renderer_api/DrawApi.h"

struct GLFWwindow;

namespace engine {

	class Application {

	public:

		static Application& getInstance();

		Application(Application const&) = delete;

		void operator=(Application const&) = delete;

		void run();

		void setViewPortSize(float newWindowWidth, float newWindowHeight);

		template<typename T>
		void initiliaze(float width, float height, const char* windowName) {
			this->windowWidth = width;
			this->windowHeight = height;
			this->windowName = windowName;
			this->initializeGlfwWindow();
			
			DrawApi::init();

#ifdef DEBUG
			DrawApi::initDebugMode();
#endif
			this->initializeImGui();

			Input::init(this->window);

			this->setScene<T>();

			this->setViewPortSize(this->windowWidth, this->windowHeight);
		}

		template<typename T>
		void setScene() {
			this->currentScene = new T();
			this->currentScene->onStartCallBack();
		}

	private:

		const char* windowName;
		float windowWidth, windowHeight;
		GLFWwindow* window;
		Scene* currentScene;
		
		Application();

		bool isRunning();

		void initializeGlfwWindow();

		void initializeImGui();
	};
}