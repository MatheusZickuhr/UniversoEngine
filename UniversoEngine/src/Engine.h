#pragma once

#include "input/Input.h"
#include "scene/Scene.h"
#include "debug/Assert.h"

struct GLFWwindow;

namespace engine {

	class Engine {

	public:

		static Engine& getInstance();

		Engine(Engine const&) = delete;

		void operator=(Engine const&) = delete;

		~Engine();

		void run();

		void setViewPortSize(float newWindowWidth, float newWindowHeight);

		template<typename T>
		void initiliaze(float width, float height, const char* windowName) {
			this->windowWidth = width;
			this->windowHeight = height;
			this->windowName = windowName;
			this->initializeGlfwWindow();

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
		
		Engine();

		bool isRunning();

		void initializeGlfwWindow();
	};
}