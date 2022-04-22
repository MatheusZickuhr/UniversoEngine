#pragma once

#include "input/Input.h"
#include "scene/Scene.h"
#include "debug/Assert.h"
#include "renderer/renderer_api/DrawApi.h"

struct GLFWwindow;

namespace engine {

	class Application {

	public:

		Application(Application const&) = delete;

		static Application& getInstance() {
			static Application instance;
			return instance;
		}

		void run();

		template<typename T>
		void initiliaze(int32_t width, int32_t height, const char* windowName) {
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

			physicsWorld = std::make_shared<PhysicsWorld>();
			renderer3d = std::make_shared<Renderer3D>();
			renderer2d = std::make_shared<Renderer2D>();
			
			this->setScene<T>();

			this->onInitialize();
		}

		template<typename T>
		void setScene() {
			this->nextScene = std::make_unique<T>();
		}

	protected:
		
		const char* windowName;
		int32_t windowWidth, windowHeight;
		GLFWwindow* window;
		std::unique_ptr<Scene> currentScene;
		std::unique_ptr<Scene> nextScene;

		std::shared_ptr<PhysicsWorld> physicsWorld;
		std::shared_ptr<Renderer3D> renderer3d;
		std::shared_ptr<Renderer2D> renderer2d;

		virtual void onImGuiRender();

		virtual void onRender();

		virtual void onInitialize();

	private:

		Application();
		
		bool isRunning();

		void initializeGlfwWindow();

		void initializeImGui();

	};
}