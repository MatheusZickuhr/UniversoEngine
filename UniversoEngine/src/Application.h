#pragma once

#include "input/Input.h"
#include "scene/Scene.h"
#include "debug/Assert.h"
#include "renderer/renderer_api/DrawApi.h"

struct GLFWwindow;

namespace engine {

	class Application {

	public:

		Application();

		Application(Application const&) = delete;

		void operator=(Application const&) = delete;

		void run();

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

			this->onInitialize();
		}

		template<typename T>
		void setScene() {
			this->currentScene = new T();
			this->currentScene->onStartCallBack();
		}

	protected:
		
		const char* windowName;
		float windowWidth, windowHeight;
		GLFWwindow* window;
		Scene* currentScene;

		virtual void onImGuiRender();

		virtual void onRender();

		virtual void onInitialize();

	private:
		
		bool isRunning();

		void initializeGlfwWindow();

		void initializeImGui();

	};
}