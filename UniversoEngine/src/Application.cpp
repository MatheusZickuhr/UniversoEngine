#include "Application.h"
#include "Application.h"
#include "utils/Time.h"

#include <algorithm>

#include <imgui.h>
#include <backends/imgui_impl_glfw.h>
#include <backends/imgui_impl_opengl3.h>

namespace engine {


	Application::Application(std::unique_ptr<Scene> scene, WindowSettings windowSettings)  {
	
		this->window = std::make_shared<Window>(windowSettings.name, windowSettings.width, windowSettings.height);

		DrawApi::init();

#ifdef DEBUG
		DrawApi::initDebugMode();
#endif

		physicsWorld = std::make_shared<PhysicsWorld>();
		renderer3d = std::make_shared<Renderer3D>();
		renderer2d = std::make_shared<Renderer2D>();

		setCurrentScene(std::move(scene));
		

		this->onInitialize();
	}

	void Application::run() {
		constexpr float fixedDeltaTime = 1.0f / 60.0f;
		// the simulation will run slower after this framerate/deltatime limit 
		constexpr float maxDeltaTime = 1.0f / 10.0f;
		
		float lastTime = 0.0f;

		float accumulator = 0.0f;

		while (window->isRunning()) {
			float currentTime = (float) getCurrentTime();
			float deltaTime = currentTime - lastTime;
			lastTime = currentTime;

			// check if the scene changed during runtime
			if (nextScene) {
				setCurrentScene(std::move(nextScene));
			}

			deltaTime = std::min(deltaTime, maxDeltaTime);

			// update the physics world
			accumulator += deltaTime;

			while (accumulator >= fixedDeltaTime) {
				this->currentScene->updatePhysicsWorld(fixedDeltaTime);
				accumulator -= fixedDeltaTime;
			}

			// update the current scene with the updated physics world data
			float timeInterpolationFactor = accumulator / fixedDeltaTime;
			this->currentScene->updatePhysics(timeInterpolationFactor);
			
			// update the current scene runtime
			this->currentScene->onUpdateCallBack(deltaTime);

			// in the end just render the current scene
			DrawApi::clearDepthAndColorBuffer(); // clean the buffers in the main frame buffer
			this->currentScene->render();
			this->onRender();

			renderImGui();

			if (window->keyPressed(Window::KEY_ESCAPE)) {
				window->close();
			}

			window->swapBuffers();
			window->pollEvents();
		}
	}

	void Application::onNewScene(std::unique_ptr<Scene> scene) {
		nextScene = std::move(scene);
	}

	void Application::renderImGui() {
		// imgui rendering
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();

		this->onImGuiRender();

		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
	}

	void Application::setCurrentScene(std::unique_ptr<Scene> scene) {
		currentScene = std::move(scene);
		currentScene->setNewSceneListener(this);
		currentScene->initialize(window, physicsWorld, renderer3d, renderer2d);
	}

	void Application::onImGuiRender() {
		
	}

	void Application::onRender() {
	}

	void Application::onInitialize() {
	}
}